//
// Created by tian wei jun on 2022/11/17 0017.
//

#ifndef CYANA_AST_RUNTIME__TOKEN_H_
#define CYANA_AST_RUNTIME__TOKEN_H_
#include <string>

#include "Grammar.h"
#include "TokenType.h"

class  __declspec(dllexport) Token {
 public:
  Token(const Grammar *terminal, int start, std::string text, TokenType type);
  ~Token();
  const Token *clone() const;

  const Grammar *const terminal;
  const int start;
  const std::string text;
  const TokenType type;
};

#endif//CYANA_AST_RUNTIME__TOKEN_H_
