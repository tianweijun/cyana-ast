//
// Created by tian wei jun on 2022/11/17 0017.
//

#ifndef CYANA_AST_RUNTIME__TOKEN_H_
#define CYANA_AST_RUNTIME__TOKEN_H_
#include <string>

#include "Grammar.h"
#include "TokenType.h"

class Token {
 public:
  Token(Grammar *terminal, int start);
  ~Token();
  const Token *clone() const;

  int start;
  std::string text;
  const Grammar *const terminal;
  TokenType type;
};

#endif//CYANA_AST_RUNTIME__TOKEN_H_
