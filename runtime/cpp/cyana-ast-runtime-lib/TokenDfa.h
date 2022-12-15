//
// Created by tian wei jun on 2022/11/25 0025.
//

#ifndef CYANA_AST__TOKENDFA_H_
#define CYANA_AST__TOKENDFA_H_

#include "TokenDfaState.h"

class TokenDfa {
 public:
  TokenDfa(const TokenDfaState *start, const TokenDfaState **states, int sizeOfStates);
  TokenDfa(TokenDfa &tokenDfa) = delete;
  TokenDfa(TokenDfa &&tokenDfa) = delete;
  ~TokenDfa();

  const TokenDfaState *const start;
  const TokenDfaState **const states;
  const int sizeOfStates;
};

#endif//CYANA_AST__TOKENDFA_H_
