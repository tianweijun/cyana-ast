//
// Created by tian wei jun on 2022/12/1 0001.
//

#ifndef CYANA_AST__PRODUCTIONRULE_H_
#define CYANA_AST__PRODUCTIONRULE_H_
#include "ByteBuffer.h"
#include "Grammar.h"
#include "SyntaxDfa.h"

class ProductionRule {
 public:
  ProductionRule();
  // notNull
  Grammar *grammar;
  ByteBuffer *alias;
  // 用于收敛产生式
  SyntaxDfa *reversedDfa;
};

#endif//CYANA_AST__PRODUCTIONRULE_H_
