//
// Created by tian wei jun on 2022/11/25 0025.
//

#ifndef CYANA_AST__DFATOKENAUTOMATA_H_
#define CYANA_AST__DFATOKENAUTOMATA_H_
#include "ByteBuffer.h"
#include "ByteBufferedInputStream.h"
#include "Token.h"
#include "TokenDfa.h"
#include <list>
#include <string>

class __declspec(dllimport) DfaTokenAutomata {
 public:
  DfaTokenAutomata(TokenDfa *tokenDfa);
  ~DfaTokenAutomata();
  std::list<Token *> *buildToken(const std::string *sourceFilePath);

 private:
  TokenDfa *dfa;
  ByteBufferedInputStream byteBufferedInputStream;
  std::list<Token *> *tokens;
  ByteBuffer oneTokenStringBuilder;
  int startIndexOfToken;
  const int eof;

 private:
  bool buildOneToken();
  TokenDfaState *getTerminalState();
};

#endif//CYANA_AST__DFATOKENAUTOMATA_H_
