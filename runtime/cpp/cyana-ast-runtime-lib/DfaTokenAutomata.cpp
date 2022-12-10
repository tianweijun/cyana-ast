//
// Created by tian wei jun on 2022/11/25 0025.
//

#include "DfaTokenAutomata.h"
#include "CyanaAstRuntimeException.h"

DfaTokenAutomata::DfaTokenAutomata(const TokenDfa *tokenDfa) : dfa(tokenDfa),
                                                               byteBufferedInputStream(ByteBufferedInputStream()),
                                                               tokens(nullptr), eof(-1),
                                                               oneTokenStringBuilder(ByteBuffer()),
                                                               startIndexOfToken(0) {
}

//dfa delete by PersistentObject.tokenDfa
//tokens delete by caller
DfaTokenAutomata::~DfaTokenAutomata() = default;

std::list<Token *> *DfaTokenAutomata::buildToken(const std::string *sourceFilePath) {
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
  const TokenDfaState *terminalState = getTerminalState();
  if (!terminalState) {// 输入流读完结束
    return false;
  }
  std::string text((char *) oneTokenStringBuilder.buffer, oneTokenStringBuilder.length());
  auto type = TokenTypeNamespace::getByGrammarAction(terminalState->terminal->action);
  auto *token = new Token(terminalState->terminal, startIndexOfToken, text, type);
  tokens->push_back(token);

  return true;
}

const TokenDfaState *DfaTokenAutomata::getTerminalState() {
  oneTokenStringBuilder.clear();
  startIndexOfToken = byteBufferedInputStream.nextReadIndex;
  int ch = byteBufferedInputStream.read();
  if (ch == eof) {
    return nullptr;
  }
  // first terminal state
  const TokenDfaState *firstTerminalState = nullptr;
  const TokenDfaState *currentState = dfa->start;
  while (ch != eof) {
    TokenDfaState *nextState = nullptr;
    auto nextStateIt = currentState->edges.find(ch);
    if (nextStateIt != currentState->edges.end()) {
      nextState = nextStateIt->second;
    }
    oneTokenStringBuilder.append((byte) ch);
    currentState = nextState;
    if (!nextState) {// 不通
      break;
    }
    if (FaStateType::isClosingTag(currentState->type)) {// 找到终态
      firstTerminalState = currentState;
      byteBufferedInputStream.mark();
      break;
    }
    ch = byteBufferedInputStream.read();
  }
  if (!firstTerminalState) {
    std::string tokenStr((char *) (oneTokenStringBuilder.buffer), oneTokenStringBuilder.position);
    throw CyanaAstRuntimeException("'" + tokenStr + "' does not match any token");
  }
  int lengthOfToken = oneTokenStringBuilder.length();
  // heaviest terminal state
  const TokenDfaState *heaviestTerminalState = firstTerminalState;
  ch = byteBufferedInputStream.read();
  while (ch != eof) {
    TokenDfaState *nextState = nullptr;
    auto nextStateIt = currentState->edges.find(ch);
    if (nextStateIt != currentState->edges.end()) {
      nextState = nextStateIt->second;
    }
    oneTokenStringBuilder.append((byte) ch);
    currentState = nextState;
    if (!nextState) {// 不通
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
  oneTokenStringBuilder.setPosition(lengthOfToken);
  return heaviestTerminalState;
}