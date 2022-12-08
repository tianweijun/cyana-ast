//
// Created by tian wei jun on 2022/12/8 0008.
//

#ifndef CYANA_AST__AUTOMATATMPAST_H_
#define CYANA_AST__AUTOMATATMPAST_H_

#include "Ast.h"
#include "Grammar.h"
#include "ProductionRule.h"
#include "Token.h"
#include <list>
#include <map>
#include <string>

class __declspec(dllimport) AutomataTmpAst {
 public:
  AutomataTmpAst(Grammar *grammar, std::string *alias);
  AutomataTmpAst(ProductionRule *productionRule);
  AutomataTmpAst(Token *token);
  ~AutomataTmpAst();

  const AutomataTmpAst *clone() const;

  const Grammar *const grammar;
  const std::string *const alias;
  // grammar.type == GrammarType.TERMINAL
  const Token *token;
  AutomataTmpAst *parent;
  std::list<AutomataTmpAst *> children;
};

class __declspec(dllimport) AutomataTmpAstCloner {
 public:
  AutomataTmpAstCloner(const AutomataTmpAst *ast);
  ~AutomataTmpAstCloner();
  AutomataTmpAst *clone();

  void cloneAst();

  const AutomataTmpAst *source;
  std::map<AutomataTmpAst *, AutomataTmpAst *> sourceDestAstMap;
  std::list<AutomataTmpAst *> sourceAsts;

 private:
  void setSourceAsts(const AutomataTmpAst *ast);
  void mapSourceDestAst();
};

class __declspec(dllimport) AutomataTmpAst2AstConverter {
 public:
  AutomataTmpAst2AstConverter(const AutomataTmpAst *ast);
  ~AutomataTmpAst2AstConverter();
  Ast *convert();
  void cloneAst();

  const AutomataTmpAst *source;
  std::map<AutomataTmpAst *, Ast *> sourceDestAstMap;
  std::list<AutomataTmpAst *> sourceAsts;

 private:
  void setSourceAsts(const AutomataTmpAst *ast);
  void mapSourceDestAst();
};

#endif//CYANA_AST__AUTOMATATMPAST_H_
