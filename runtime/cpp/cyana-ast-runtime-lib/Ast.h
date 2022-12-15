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

class __declspec(dllexport) Ast {
 public:
  Ast(const Grammar *grammar, const std::string *alias);
  explicit Ast(const Token *token);
  Ast(Ast &ast) = delete;
  Ast(Ast &&ast) = delete;
  ~Ast();

  const Ast *clone() const;
  std::string *toString() const;

  const Grammar *const grammar;
  const std::string *const alias;
  // grammar.type == GrammarType.TERMINAL
  const Token *token;
  const Ast *parent;
  std::list<Ast *> children;
};

class AstCloner {
 public:
  explicit AstCloner(const Ast *ast);
  AstCloner(AstCloner &astCloner) = delete;
  AstCloner(AstCloner &&astCloner) = delete;
  ~AstCloner();
  Ast *clone();

 private:
  const Ast *source;
  std::map<const Ast *, Ast *> sourceDestAstMap;
  std::list<Ast *> sourceAsts;

 private:
  void setSourceAsts(const Ast *ast);
  void mapSourceDestAst();
  void cloneAst();
};

#endif//CYANA_AST_RUNTIME__AST_H_
