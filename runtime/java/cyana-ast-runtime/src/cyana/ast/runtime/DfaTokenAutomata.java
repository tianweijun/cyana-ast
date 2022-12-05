package cyana.ast.runtime;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.LinkedList;
import java.util.List;

/**
 * 识别token的dfa.
 *
 * @author tian wei jun
 */
public class DfaTokenAutomata {
  TokenDfa dfa;
  int eof = -1;
  ByteBufferedInputStream byteBufferedInputStream;

  LinkedList<Token> tokens;
  StringBuilder oneTokenStringBuilder;
  int startIndexOfToken;

  /**
   * 初始化oneTokenStringBuilder、eof、dfa.
   *
   * @param dfa 识别token的 确定有限状态自动机
   */
  public DfaTokenAutomata(TokenDfa dfa) {
    this.dfa = dfa;
    this.oneTokenStringBuilder = new StringBuilder();
  }

  public List<Token> buildToken(String sourceFilePath) {
    FileInputStream fileInputStream = null;
    List<Token> tokens = null;
    try {
      fileInputStream = new FileInputStream(sourceFilePath);
      tokens = buildToken(fileInputStream);
    } catch (Exception e) {
      throw new CyanaAstRuntimeException(e);
    } finally {
      if (null != fileInputStream) {
        try {
          fileInputStream.close();
        } catch (IOException e) {
          throw new CyanaAstRuntimeException(e);
        }
      }
    }
    return tokens;
  }

  /**
   * 根据输入流构造tokens.
   *
   * @param inputStream 将要识别文本的输入流.
   * @return
   */
  public List<Token> buildToken(InputStream inputStream) {
    byteBufferedInputStream = new ByteBufferedInputStream(inputStream);
    try {
      this.tokens = new LinkedList<>();
      boolean hasBuildedToken = false;
      do {
        hasBuildedToken = buildOneToken();
      } while (hasBuildedToken);
    } finally {
      byteBufferedInputStream.close();
    }
    return tokens;
  }

  /**
   * 一开始处于初态，每吃一个字符转到下一个状态.
   *
   * @return boolean 是否完成一个token的构造，是的话返回true
   */
  private boolean buildOneToken() {
    TokenDfaState terminalState = getTerminalState();
    if (null == terminalState) { // 输入流读完结束
      return false;
    }
    Token token = new Token(startIndexOfToken);
    token.text = oneTokenStringBuilder.toString();
    token.terminal = terminalState.terminal;
    token.type = TokenType.getByGrammarAction(terminalState.terminal.action);
    tokens.add(token);

    return true;
  }

  private TokenDfaState getTerminalState() {
    oneTokenStringBuilder.delete(0, oneTokenStringBuilder.length());
    startIndexOfToken = byteBufferedInputStream.nextReadIndex;
    int ch = byteBufferedInputStream.read();
    if (ch == eof) {
      return null;
    }
    // first terminal state
    TokenDfaState firstTerminalState = null;
    TokenDfaState currentState = dfa.start;
    while (ch != eof) {
      TokenDfaState nextState = currentState.edges.get(ch);
      oneTokenStringBuilder.append((char) ch);
      currentState = nextState;
      if (null == nextState) { // 不通
        break;
      }
      if (FaStateType.isClosingTag(currentState.type)) { // 找到终态
        firstTerminalState = currentState;
        byteBufferedInputStream.mark();
        break;
      }
      ch = byteBufferedInputStream.read();
    }
    if (null == firstTerminalState) {
      throw new CyanaAstRuntimeException(
          String.format("'%s' does not match any token", oneTokenStringBuilder.toString()));
    }
    int lengthOfToken = oneTokenStringBuilder.length();
    // heaviest terminal state
    TokenDfaState heaviestTerminalState = firstTerminalState;
    ch = byteBufferedInputStream.read();
    while (ch != eof) {
      TokenDfaState nextState = currentState.edges.get(ch);
      oneTokenStringBuilder.append((char) ch);
      currentState = nextState;
      if (null == nextState) { // 不通
        break;
      }
      if (FaStateType.isClosingTag(currentState.type)) { // 找到终态
        // 总是贪婪的
        if (currentState.weight >= heaviestTerminalState.weight) {
          heaviestTerminalState = currentState;
          lengthOfToken = oneTokenStringBuilder.length();
          byteBufferedInputStream.mark();
        }
      }
      ch = byteBufferedInputStream.read();
    }
    byteBufferedInputStream.reset();
    oneTokenStringBuilder.delete(lengthOfToken, oneTokenStringBuilder.length());
    return heaviestTerminalState;
  }
}
