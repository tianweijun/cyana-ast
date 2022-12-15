//
// Created by tian wei jun on 2022/11/17 0017.
//

#ifndef CYANA_AST_RUNTIME__GRAMMAR_H_
#define CYANA_AST_RUNTIME__GRAMMAR_H_
#include "GrammarAction.h"
#include "GrammarType.h"
#include <string>
class __declspec(dllexport) Grammar {
 public:
  explicit Grammar(const std::string *name, const GrammarType type, const GrammarAction action);
  Grammar(const Grammar &grammar) = delete;
  Grammar(const Grammar &&grammar) = delete;
  ~Grammar();

  const std::string *const name;
  const GrammarType type;
  const GrammarAction action;
};

#endif//CYANA_AST_RUNTIME__GRAMMAR_H_
