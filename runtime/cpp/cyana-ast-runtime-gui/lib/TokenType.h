//
// Created by tian wei jun on 2022/11/17 0017.
//

#ifndef CYANA_AST_RUNTIME__TOKENTYPE_H_
#define CYANA_AST_RUNTIME__TOKENTYPE_H_

#include "GrammarAction.h"

enum class  TokenType : int {
  TEXT = 0,
  SKIP = 1
};

namespace TokenTypeNamespace {
__declspec(dllimport) TokenType getByGrammarAction(GrammarAction grammarAction);
}// namespace TokenTypeNamespace
#endif//CYANA_AST_RUNTIME__TOKENTYPE_H_
