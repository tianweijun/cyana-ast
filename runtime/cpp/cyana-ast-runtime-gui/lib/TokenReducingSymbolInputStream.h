//
// Created by tian wei jun on 2022/12/4 0004.
//

#ifndef CYANA_AST__TOKENREDUCINGSYMBOLINPUTSTREAM_H_
#define CYANA_AST__TOKENREDUCINGSYMBOLINPUTSTREAM_H_

#include "Token.h"
#include <list>

class __declspec(dllimport) TokenReducingSymbolInputStream {
 public:
  TokenReducingSymbolInputStream(std::list<Token *> *sourceTokens);
  ~TokenReducingSymbolInputStream();

  Token **tokenReducingSymbols;
  int sizeOfTokenReducingSymbols;
  int nextReadIndex;

  Token *read();
  bool hasNext();
  bool hasReadAll();

 private:
  void setTokenReducingSymbols(std::list<Token *> *sourceToken);
};

#endif//CYANA_AST__TOKENREDUCINGSYMBOLINPUTSTREAM_H_
