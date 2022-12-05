//
// Created by tian wei jun on 2022/11/25 0025.
//

#include "SyntaxDfa.h"

SyntaxDfa::SyntaxDfa() : start(0), states(0), sizeOfStates(0) {
}

SyntaxDfa::~SyntaxDfa() {
  for (int i = 0; i < sizeOfStates; i++) {
    SyntaxDfaState *syntaxDfaState = states[i];
    delete syntaxDfaState;
    syntaxDfaState = 0;
  }
  delete[] states;
  states = 0;
}