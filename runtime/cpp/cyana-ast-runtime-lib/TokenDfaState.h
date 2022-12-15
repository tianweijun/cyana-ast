//
// Created by tian wei jun on 2022/11/30 0030.
//

#ifndef CYANA_AST__TOKENDFASTATE_H_
#define CYANA_AST__TOKENDFASTATE_H_

#include "FaStateType.h"
#include "Grammar.h"
#include <map>

using byte = uint8_t;

class TokenDfaState {
 public:
  TokenDfaState();
  TokenDfaState(const TokenDfaState &tokenDfaState) = delete;
  TokenDfaState(const TokenDfaState &&tokenDfaState) = delete;
  ~TokenDfaState();

  int type;
  int weight;
  // token语法名字
  Grammar *terminal;
  // 转移
  std::map<byte, TokenDfaState *> edges;
};

#endif//CYANA_AST__TOKENDFASTATE_H_
