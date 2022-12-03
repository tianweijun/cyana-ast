//
// Created by tian wei jun on 2022/12/1 0001.
//

#ifndef CYANA_AST__SYNTAXDFASTATE_H_
#define CYANA_AST__SYNTAXDFASTATE_H_
#include "Grammar.h"
#include "ProductionRule.h"
#include <list>
#include <map>

class SyntaxDfaStateEdgesCompare {
 public:
  bool operator()(const Grammar *t1, const Grammar *t2) {
    const std::string *const name1 = t1->name;
    const std::string *const name2 = t2->name;
    if (name1->length() != name2->length()) {
      return name1->length() < name2->length();
    }
    bool ret = false;
    for (int i = 0; i < name1->length(); i++) {
      if (name1[i] != name2[i]) {
        ret = name1[1] < name2[i];
        break;
      }
    }
    return ret;
  }
};

class SyntaxDfaState {
 public:
  SyntaxDfaState();

  int type;
  // 转移
  std::map<Grammar *, SyntaxDfaState *, SyntaxDfaStateEdgesCompare> edges;
  std::list<ProductionRule *> closingProductionRules;
};

#endif//CYANA_AST__SYNTAXDFASTATE_H_
