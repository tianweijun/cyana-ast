//
// Created by tian wei jun on 2022/11/25 0025.
//

#include "DfaTokenAutomata.h"
#include "CyanaAstRuntimeException.h"

DfaTokenAutomata::DfaTokenAutomata(TokenDfa *tokenDfa) : dfa(tokenDfa),
                                                         byteBufferedInputStream(ByteBufferedInputStream()),
                                                         tokens(0), eof(-1),
                                                         oneTokenStringBuilder(ByteBuffer()),
                                                         startIndexOfToken(0) {
}

DfaTokenAutomata::~DfaTokenAutomata() {
  //dfa delete by PersistentObject.tokenDfa
  //tokens delete by caller
}

std::list<Token *> *DfaTokenAutomata::buildToken(const std::string sourceFilePath) {
  byteBufferedInputStream.init(sourceFilePath);
  //tokens delete by caller
  this->tokens = new std::list<Token *>();
  bool hasBuildedToken = false;
  do {
    hasBuildedToken = buildOneToken();
  } while (hasBuildedToken);
  return this->tokens;
}

bool DfaTokenAutomata::buildOneToken() {
  TokenDfaState *terminalState = getTerminalState();
  if (0 == terminalState) {// 输入流读完结束
    return false;
  }
  Token *token = new Token(terminalState->terminal, startIndexOfToken);
  token->type = TokenTypeNamespace::getByGrammarAction(terminalState->terminal->action);
  token->text.append((char *) oneTokenStringBuilder.buffer, oneTokenStringBuilder.length());
  tokens->push_back(token);

  return true;
}

TokenDfaState *DfaTokenAutomata::getTerminalState() {
  oneTokenStringBuilder.clear();
  startIndexOfToken = byteBufferedInputStream.nextReadIndex;
  int ch = byteBufferedInputStream.read();
  if (ch == eof) {
    return 0;
  }
  // first terminal state
  TokenDfaState *firstTerminalState = 0;
  TokenDfaState *currentState = dfa->start;
  while (ch != eof) {
    TokenDfaState *nextState = 0;
    auto nextStateIt = currentState->edges.find(ch);
    if (nextStateIt != currentState->edges.end()) {
      nextState = nextStateIt->second;
    }
    oneTokenStringBuilder.append((byte) ch);
    currentState = nextState;
    if (0 == nextState) {// 不通
      break;
    }
    if (FaStateType::isClosingTag(currentState->type)) {// 找到终态
      firstTerminalState = currentState;
      byteBufferedInputStream.mark();
      break;
    }
    ch = byteBufferedInputStream.read();
  }
  if (0 == firstTerminalState) {
    std::string tokenStr((char *) (oneTokenStringBuilder.buffer), oneTokenStringBuilder.position);
    throw new CyanaAstRuntimeException("'" + tokenStr + "' does not match any token");
  }
  int lengthOfToken = oneTokenStringBuilder.length();
  // heaviest terminal state
  TokenDfaState *heaviestTerminalState = firstTerminalState;
  ch = byteBufferedInputStream.read();
  while (ch != eof) {
    TokenDfaState *nextState = 0;
    auto nextStateIt = currentState->edges.find(ch);
    if (nextStateIt != currentState->edges.end()) {
      nextState = nextStateIt->second;
    }
    oneTokenStringBuilder.append((byte) ch);
    currentState = nextState;
    if (0 == nextState) {// 不通
      break;
    }
    if (FaStateType::isClosingTag(currentState->type)) {// 找到终态
      // 总是贪婪的
      if (currentState->weight >= heaviestTerminalState->weight) {
        heaviestTerminalState = currentState;
        lengthOfToken = oneTokenStringBuilder.length();
        byteBufferedInputStream.mark();
      }
    }
    ch = byteBufferedInputStream.read();
  }
  byteBufferedInputStream.reset();
  oneTokenStringBuilder.limit(lengthOfToken);
  return heaviestTerminalState;
}
