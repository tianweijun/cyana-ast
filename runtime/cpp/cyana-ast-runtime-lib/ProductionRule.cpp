//
// Created by tian wei jun on 2022/12/1 0001.
//

#include "ProductionRule.h"

ProductionRule::ProductionRule() : grammar(nullptr), alias(nullptr), reversedDfa(nullptr) {
}

ProductionRule::~ProductionRule() {
  //grammar delete by PersistentData.grammars
  //alias delete by PersistentData.stringPool
  delete reversedDfa;
  reversedDfa = nullptr;
}
