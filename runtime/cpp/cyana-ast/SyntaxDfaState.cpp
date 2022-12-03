//
// Created by tian wei jun on 2022/12/1 0001.
//

#include "SyntaxDfaState.h"

SyntaxDfaState::SyntaxDfaState() : type(0),
                                   edges(std::map<Grammar *, SyntaxDfaState *, SyntaxDfaStateEdgesCompare>()) {
}