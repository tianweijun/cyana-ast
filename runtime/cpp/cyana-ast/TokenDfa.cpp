//
// Created by tian wei jun on 2022/11/25 0025.
//

#include "TokenDfa.h"

TokenDfa::TokenDfa() : start(0), states(0), sizeOfStates(0) {
}

TokenDfa::~TokenDfa() {
  for (int i = 0; i < sizeOfStates; i++) {
    TokenDfaState *tokenDfaState = states[i];
    delete tokenDfaState;
    tokenDfaState = 0;
  }
  delete states;
  states = 0;
}
