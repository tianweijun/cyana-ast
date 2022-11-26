//
// Created by tian wei jun on 2022/11/17 0017.
//

#ifndef CYANA_AST_RUNTIME__AST_H_
#define CYANA_AST_RUNTIME__AST_H_
#include "AstType.h"
#include "Token.h"
#include <list>
#include <string>
class Ast {
 public:
  Ast();

  std::string name;

  std::string alias;
  AstType type;
  // type == AstType.TERMINAL
  const Token *const token;
  Ast *parent;
  std::list<Ast *> children;

  std::string* newString();
};

#endif//CYANA_AST_RUNTIME__AST_H_
