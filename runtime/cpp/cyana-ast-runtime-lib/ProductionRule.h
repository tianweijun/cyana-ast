//
// Created by tian wei jun on 2022/12/1 0001.
//

#ifndef CYANA_AST__PRODUCTIONRULE_H_
#define CYANA_AST__PRODUCTIONRULE_H_
#include "Grammar.h"
#include "SyntaxDfa.h"
#include <string>

class SyntaxDfa;

class ProductionRule {
 public:
  ProductionRule();
  ProductionRule(ProductionRule &productionRule) = delete;
  ProductionRule(ProductionRule &&productionRule) = delete;
  ~ProductionRule();
  // notNull
  const Grammar *grammar;
  const std::string *alias;
  // 用于收敛产生式
  const SyntaxDfa *reversedDfa;
};

#endif//CYANA_AST__PRODUCTIONRULE_H_
