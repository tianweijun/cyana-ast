package cyana.ast.runtime;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * PersistentData对应在文件中的对象.
 *
 * @author tian wei jun
 */
public class PersistentData {
  InputStream inputStream;
  String[] stringPool;
  Grammar[] grammars;
  ProductionRule[] productionRules;

  ByteBuffer intByteBuffer = ByteBuffer.allocate(4);

  public PersistentData() {
    intByteBuffer.order(ByteOrder.BIG_ENDIAN);
  }

  public PersistentData(String persistentDataFilePath) {
    this();
    init(persistentDataFilePath);
  }

  private void init(String persistentDataFilePath) {
    try {
      this.inputStream = new FileInputStream(persistentDataFilePath);
    } catch (Exception e) {
      throw new CyanaAstRuntimeException(e);
    }
  }

  public PersistentData(InputStream inputStream) {
    this();
    this.inputStream = inputStream;
  }

  /**
   * 只能调用一次.
   *
   * @return
   */
  public String[] getStringPoolByInputStream() {
    int sizeOfString = readInt();
    String[] strings = new String[sizeOfString];
    for (int indexOfStrings = 0; indexOfStrings < sizeOfString; indexOfStrings++) {
      int countOfStringBytes = readInt();
      String str = readString(countOfStringBytes);
      strings[indexOfStrings] = str;
    }
    this.stringPool = strings;
    return strings;
  }

  /**
   * 只能调用一次.
   *
   * @return
   */
  public Grammar[] getGrammarsByInputStream() {
    int sizeOfGramamr = readInt();
    Grammar[] grammars = new Grammar[sizeOfGramamr];

    GrammarType[] grammarTypes = GrammarType.values();
    GrammarAction[] grammarActions = GrammarAction.values();
    for (int indexOfGrammars = 0; indexOfGrammars < sizeOfGramamr; indexOfGrammars++) {
      Grammar grammar = new Grammar();
      grammar.name = stringPool[readInt()];
      grammar.type = grammarTypes[readInt()];
      grammar.action = grammarActions[readInt()];
      grammars[indexOfGrammars] = grammar;
    }
    this.grammars = grammars;
    return grammars;
  }

  /**
   * 只能调用一次.
   *
   * @return
   */
  public TokenDfa getTokenDfaByInputStream() {
    int sizeOfTokenDfaStates = readInt();
    TokenDfaState[] tokenDfaStates = new TokenDfaState[sizeOfTokenDfaStates];
    for (int indexOfTokenDfaStates = 0;
        indexOfTokenDfaStates < sizeOfTokenDfaStates;
        indexOfTokenDfaStates++) {
      tokenDfaStates[indexOfTokenDfaStates] = new TokenDfaState();
    }
    // countOfTokenDfaStates-(type-weight-terminal-countOfEdges-[ch,dest]{countOfEdges})
    for (int indexOfTokenDfaStates = 0;
        indexOfTokenDfaStates < sizeOfTokenDfaStates;
        indexOfTokenDfaStates++) {
      TokenDfaState tokenDfaState = tokenDfaStates[indexOfTokenDfaStates];
      tokenDfaState.type = readInt();
      tokenDfaState.weight = readInt();
      int intOfTerminal = readInt();
      if (intOfTerminal >= 0) {
        tokenDfaState.terminal = grammars[intOfTerminal];
      }
      int sizeOfEdges = readInt();
      for (int indexOfEdges = 0; indexOfEdges < sizeOfEdges; indexOfEdges++) {
        int ch = readInt();
        TokenDfaState chToState = tokenDfaStates[readInt()];
        tokenDfaState.edges.put(ch, chToState);
      }
    }
    TokenDfa tokenDfa = new TokenDfa();
    tokenDfa.start = tokenDfaStates[0];
    return tokenDfa;
  }

  public Grammar getStartGrammarByInputStream() {
    int indexOfStartGrammar = readInt();
    return grammars[indexOfStartGrammar];
  }

  public void getProductionRulesByInputStream() {
    int countOfProductionRules = readInt();
    ProductionRule[] productionRules = new ProductionRule[countOfProductionRules];
    for (int indexOfProductionRules = 0;
        indexOfProductionRules < countOfProductionRules;
        indexOfProductionRules++) {
      productionRules[indexOfProductionRules] = new ProductionRule();
    }
    this.productionRules = productionRules;

    for (ProductionRule productionRule : productionRules) {
      productionRule.grammar = grammars[readInt()];
      int indexOfAliasInStringPool = readInt();
      if (indexOfAliasInStringPool >= 0) {
        productionRule.alias = stringPool[indexOfAliasInStringPool];
      }
      productionRule.reversedDfa = getSyntaxDfaByInputStream();
    }
  }

  public SyntaxDfa getSyntaxDfaByInputStream() {
    int sizeOfSyntaxDfaStates = readInt();
    SyntaxDfaState[] syntaxDfaStates = new SyntaxDfaState[sizeOfSyntaxDfaStates];
    for (int indexOfSyntaxDfaStates = 0;
        indexOfSyntaxDfaStates < sizeOfSyntaxDfaStates;
        indexOfSyntaxDfaStates++) {
      syntaxDfaStates[indexOfSyntaxDfaStates] = new SyntaxDfaState();
    }
    // countOfSyntaxDfaStates-(type-countOfEdges-[ch,dest]{countOfEdges}-countOfProductions-productions)
    for (int indexOfSyntaxDfaStates = 0;
        indexOfSyntaxDfaStates < sizeOfSyntaxDfaStates;
        indexOfSyntaxDfaStates++) {
      SyntaxDfaState syntaxDfaState = syntaxDfaStates[indexOfSyntaxDfaStates];
      syntaxDfaState.type = readInt();
      int sizeOfEdges = readInt();
      for (int indexOfEdges = 0; indexOfEdges < sizeOfEdges; indexOfEdges++) {
        Grammar ch = grammars[readInt()];
        SyntaxDfaState chToState = syntaxDfaStates[readInt()];
        syntaxDfaState.edges.put(ch, chToState);
      }
      int sizeOfProductions = readInt();
      for (int indexOfProductions = 0;
          indexOfProductions < sizeOfProductions;
          indexOfProductions++) {
        syntaxDfaState.closingProductionRules.add(productionRules[readInt()]);
      }
    }
    SyntaxDfa syntaxDfa = new SyntaxDfa();
    syntaxDfa.start = syntaxDfaStates[0];
    return syntaxDfa;
  }

  private String readString(int countOfStringBytes) {
    byte[] bytes = new byte[countOfStringBytes];
    doRead(bytes, 0, countOfStringBytes);

    char[] chars = new char[countOfStringBytes];
    int indexOfChars = 0;
    while (indexOfChars < countOfStringBytes) {
      byte b = bytes[indexOfChars];
      chars[indexOfChars] = (char) b;
      ++indexOfChars;
    }
    return new String(chars);
  }

  private int readInt() {
    doRead(intByteBuffer.array(), 0, intByteBuffer.capacity());
    intByteBuffer.clear();
    intByteBuffer.limit(intByteBuffer.capacity());
    return intByteBuffer.getInt();
  }

  private void doRead(byte[] bytes, int offset, int length) {
    try {
      inputStream.read(bytes, offset, length);
    } catch (Exception e) {
      throw new CyanaAstRuntimeException(e);
    }
  }

  public void closeInputStream() {
    if (null != inputStream) {
      try {
        inputStream.close();
      } catch (IOException e) {
        throw new CyanaAstRuntimeException(e);
      }
    }
  }

  public void compact() {
    closeInputStream();
    intByteBuffer = null;
  }
}
