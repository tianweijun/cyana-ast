//
// Created by tian wei jun on 2022/11/17 0017.
//

#ifndef CYANA_AST_RUNTIME__AST_H_
#define CYANA_AST_RUNTIME__AST_H_

#include "Grammar.h"
#include "ProductionRule.h"
#include "Token.h"
#include <list>
#include <map>
#include <string>

class __declspec(dllimport) Ast {
 public:
  Ast(Grammar *grammar, std::string *alias);
  Ast(ProductionRule *productionRule);
  Ast(Token *token);
  ~Ast();

  const Ast *clone() const;
  std::string *newString() const;

  const Grammar *const grammar;
  const std::string *const alias;
  // grammar.type == GrammarType.TERMINAL
  const Token *token;
  Ast *parent;
  std::list<Ast *> children;
};

class __declspec(dllimport) AstCloner {
 public:
  AstCloner(const Ast *ast);
  ~AstCloner();
  Ast *clone();
  void cloneAst();

  const Ast *source;
  std::map<Ast *, Ast *> sourceDestAstMap;
  std::list<Ast *> sourceAsts;

 private:
  void setSourceAsts(const Ast *ast);
  void mapSourceDestAst();
};

#endif//CYANA_AST_RUNTIME__AST_H_
