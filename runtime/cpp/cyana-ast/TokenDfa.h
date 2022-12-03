//
// Created by tian wei jun on 2022/11/25 0025.
//

#ifndef CYANA_AST__TOKENDFA_H_
#define CYANA_AST__TOKENDFA_H_

#include "TokenDfaState.h"

class TokenDfa {
 public:
  TokenDfa();
  ~TokenDfa();

  TokenDfaState *start;
  TokenDfaState **states;
  int sizeOfStates;
};

#endif//CYANA_AST__TOKENDFA_H_
