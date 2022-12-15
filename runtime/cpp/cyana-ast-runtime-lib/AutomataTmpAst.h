//
// Created by tian wei jun on 2022/12/8 0008.
//

#ifndef CYANA_AST__AUTOMATATMPAST_H_
#define CYANA_AST__AUTOMATATMPAST_H_

#include "Ast.h"
#include "Grammar.h"
#include "Token.h"
#include <list>
#include <map>
#include <string>

class AutomataTmpAst {
 public:
  AutomataTmpAst(const Grammar *grammar, const std::string *alias);
  explicit AutomataTmpAst(const Token *token);
  AutomataTmpAst(const AutomataTmpAst &automataTmpAst) = delete;
  AutomataTmpAst(const AutomataTmpAst &&automataTmpAst) = delete;
  ~AutomataTmpAst();

  const AutomataTmpAst *clone() const;

  const Grammar *const grammar;
  const std::string *const alias;
  // grammar.type == GrammarType.TERMINAL
  const Token *token;
  const AutomataTmpAst *parent;
  std::list<AutomataTmpAst *> children;
};

class AutomataTmpAstCloner {
 public:
  explicit AutomataTmpAstCloner(const AutomataTmpAst *ast);
  AutomataTmpAstCloner(const AutomataTmpAstCloner &automataTmpAstCloner) = delete;
  AutomataTmpAstCloner(const AutomataTmpAstCloner &&automataTmpAstCloner) = delete;
  ~AutomataTmpAstCloner();
  AutomataTmpAst *clone();

 private:
  void cloneAst();

 private:
  const AutomataTmpAst *source;
  std::map<const AutomataTmpAst *, AutomataTmpAst *> sourceDestAstMap;
  std::list<AutomataTmpAst *> sourceAsts;

 private:
  void setSourceAsts(const AutomataTmpAst *ast);
  void mapSourceDestAst();
};

class AutomataTmpAst2AstConverter {
 public:
  explicit AutomataTmpAst2AstConverter(const AutomataTmpAst *ast);
  AutomataTmpAst2AstConverter(const AutomataTmpAst2AstConverter &automataTmpAst2AstConverter) = delete;
  AutomataTmpAst2AstConverter(const AutomataTmpAst2AstConverter &&automataTmpAst2AstConverter) = delete;
  ~AutomataTmpAst2AstConverter();
  Ast *convert();

 private:
  const AutomataTmpAst *source;
  std::map<const AutomataTmpAst *, Ast *> sourceDestAstMap;
  std::list<AutomataTmpAst *> sourceAsts;

 private:
  void cloneAst();
  void setSourceAsts(const AutomataTmpAst *ast);
  void mapSourceDestAst();
};

#endif//CYANA_AST__AUTOMATATMPAST_H_
