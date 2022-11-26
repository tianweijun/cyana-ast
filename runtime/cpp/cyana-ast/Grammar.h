//
// Created by tian wei jun on 2022/11/17 0017.
//

#ifndef CYANA_AST_RUNTIME__GRAMMAR_H_
#define CYANA_AST_RUNTIME__GRAMMAR_H_
#include "GrammarAction.h"
#include "GrammarType.h"
#include <string>
class Grammar {
 public:
  Grammar();
  std::string name;
  GrammarType type;
  GrammarAction action;
};

#endif//CYANA_AST_RUNTIME__GRAMMAR_H_
