//
// Created by tian wei jun on 2022/12/1 0001.
//

#ifndef CYANA_AST__SYNTAXDFASTATE_H_
#define CYANA_AST__SYNTAXDFASTATE_H_
#include "Grammar.h"
#include "ProductionRule.h"
#include <list>
#include <map>

class ProductionRule;

class SyntaxDfaState {
 public:
  SyntaxDfaState();
  ~SyntaxDfaState();

  int type;
  // 转移
  std::map<const Grammar *, SyntaxDfaState *> edges;
  std::list<ProductionRule *> closingProductionRules;
};

#endif//CYANA_AST__SYNTAXDFASTATE_H_
