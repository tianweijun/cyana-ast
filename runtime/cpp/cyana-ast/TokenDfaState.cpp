//
// Created by tian wei jun on 2022/11/30 0030.
//

#include "TokenDfaState.h"

TokenDfaState::TokenDfaState() : type((int) FaStateEnumType::NONE),
                                 weight(0), terminal(0),
                                 edges(std::map<byte, TokenDfaState *>()) {
}
TokenDfaState::~TokenDfaState() {
  //terminal delete by PersistentData.grammars
  //TokenDfaState in edges delete by TokenDfa
}