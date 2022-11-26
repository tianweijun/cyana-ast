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
  Token(int start);
  
  int start;
  std::string text;
  Grammar terminal;
  TokenType type;
};

#endif//CYANA_AST_RUNTIME__TOKEN_H_
