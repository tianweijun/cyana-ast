//
// Created by tian wei jun on 2022/11/25 0025.
//

#ifndef CYANA_AST__SYNTAXDFA_H_
#define CYANA_AST__SYNTAXDFA_H_

#include "SyntaxDfaState.h"

class SyntaxDfaState;

class SyntaxDfa {
 public:
  SyntaxDfa(const SyntaxDfaState *start, const SyntaxDfaState **states, const int sizeOfStates);
  ~SyntaxDfa();

  const SyntaxDfaState *const start;
  const SyntaxDfaState **const states;
  const int sizeOfStates;
};

#endif//CYANA_AST__SYNTAXDFA_H_
