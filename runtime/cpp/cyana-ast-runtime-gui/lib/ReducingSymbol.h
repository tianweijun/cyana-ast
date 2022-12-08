//
// Created by tian wei jun on 2022/12/4 0004.
//

#ifndef CYANA_AST__REDUCINGSYMBOL_H_
#define CYANA_AST__REDUCINGSYMBOL_H_

#include "AutomataTmpAst.h"
#include "Grammar.h"
#include "SyntaxDfaState.h"

class __declspec(dllimport) ReducingSymbol {
 public:
  ReducingSymbol();
  ~ReducingSymbol();

  // for BacktrackingBottomUpAstAutomata.triedBottomUpBranchs(set)
  bool compare(ReducingSymbol *o);
  // for BacktrackingBottomUpAstAutomata.triedBottomUpBranchs(set)
  bool equals(ReducingSymbol *o);
  ReducingSymbol *clone();
  // for BacktrackingBottomUpAstAutomata.triedBottomUpBranchs(set)
  ReducingSymbol *uniqueIdentificationClone();

  // grammar
  const Grammar *reducedGrammar;
  // ast
  const AutomataTmpAst *astOfCurrentDfaState;
  // 状态
  const SyntaxDfaState *currentDfaState;
  // token流中的位置
  int endIndexOfToken;
};

#endif//CYANA_AST__REDUCINGSYMBOL_H_
