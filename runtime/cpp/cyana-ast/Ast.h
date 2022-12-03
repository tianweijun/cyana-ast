//
// Created by tian wei jun on 2022/11/17 0017.
//

#ifndef CYANA_AST_RUNTIME__AST_H_
#define CYANA_AST_RUNTIME__AST_H_

#include "Grammar.h"
#include "Token.h"
#include <list>
#include <string>

class Ast {
 public:
  Ast(Grammar *grammar, std::string *alias);
  ~Ast();

  const Grammar *const grammar;

  const std::string *const alias;
  // grammar.type == GrammarType.TERMINAL
  const Token *token;
  Ast *parent;
  std::list<Ast *> children;

  std::string *newString();
};

#endif//CYANA_AST_RUNTIME__AST_H_
