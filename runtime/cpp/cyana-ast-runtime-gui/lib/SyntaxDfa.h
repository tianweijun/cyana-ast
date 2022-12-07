//
// Created by tian wei jun on 2022/11/25 0025.
//

#ifndef CYANA_AST__SYNTAXDFA_H_
#define CYANA_AST__SYNTAXDFA_H_

#include "SyntaxDfaState.h"

class  SyntaxDfaState;

class __declspec(dllimport) SyntaxDfa {
 public:
  SyntaxDfa();
  ~SyntaxDfa();

  SyntaxDfaState *start;
  SyntaxDfaState **states;
  int sizeOfStates;
};

#endif//CYANA_AST__SYNTAXDFA_H_
