//
// Created by tian wei jun on 2022/12/4 0004.
//

#ifndef CYANA_AST__REDUCINGSYMBOL_H_
#define CYANA_AST__REDUCINGSYMBOL_H_

#include "AutomataTmpAst.h"
#include "Grammar.h"
#include "SyntaxDfaState.h"

class ReducingSymbol {
 public:
  ReducingSymbol();
  ReducingSymbol(const ReducingSymbol &reducingSymbol) = delete;
  ReducingSymbol(const ReducingSymbol &&reducingSymbol) = delete;
  ~ReducingSymbol();

  // for BacktrackingBottomUpAstAutomata.triedBottomUpBranchs(set)
  bool compare(const ReducingSymbol *o) const;
  // for BacktrackingBottomUpAstAutomata.triedBottomUpBranchs(set)
  bool equals(const ReducingSymbol *o) const;
  ReducingSymbol *clone() const;
  // for BacktrackingBottomUpAstAutomata.triedBottomUpBranchs(set)
  ReducingSymbol *uniqueIdentificationClone() const;

  // grammar
  const Grammar *reducedGrammar;
  // ast
  const AutomataTmpAst *astOfCurrentDfaState;
  // 状态
  const SyntaxDfaState *currentDfaState;
  // token流中的位置
  unsigned int endIndexOfToken;
};

#endif//CYANA_AST__REDUCINGSYMBOL_H_
