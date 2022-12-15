//
// Created by tian wei jun on 2022/12/4 0004.
//

#ifndef CYANA_AST__TOKENREDUCINGSYMBOLINPUTSTREAM_H_
#define CYANA_AST__TOKENREDUCINGSYMBOLINPUTSTREAM_H_

#include "Token.h"
#include <list>

class TokenReducingSymbolInputStream {
 public:
  TokenReducingSymbolInputStream();
  explicit TokenReducingSymbolInputStream(std::list<Token *> *sourceTokens);
  TokenReducingSymbolInputStream(TokenReducingSymbolInputStream &tokenReducingSymbolInputStream) = delete;
  TokenReducingSymbolInputStream(TokenReducingSymbolInputStream &&tokenReducingSymbolInputStream) = delete;
  ~TokenReducingSymbolInputStream();

  Token **tokenReducingSymbols;
  unsigned int sizeOfTokenReducingSymbols;
  unsigned int nextReadIndex;

  Token *read();
  bool hasNext() const;
  bool hasReadAll() const;
  void clear();
  void init(std::list<Token *> *sourceToken);
};

#endif//CYANA_AST__TOKENREDUCINGSYMBOLINPUTSTREAM_H_
