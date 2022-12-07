//
// Created by tian wei jun on 2022/12/4 0004.
//

#ifndef CYANA_AST__BACKTRACKINGBOTTOMUPBRANCH_H_
#define CYANA_AST__BACKTRACKINGBOTTOMUPBRANCH_H_

#include "ReducingSymbol.h"
#include <list>

enum class  BacktrackingBottomUpBranchStatus : int {
  CREATED = 0,
  REDUCED,
  SHIFTED,
  NON_ACCEPTED,
  ACCEPTED
};

class __declspec(dllimport) BacktrackingBottomUpBranch {
 public:
  BacktrackingBottomUpBranch();
  ~BacktrackingBottomUpBranch();

  BacktrackingBottomUpBranch *clone() const;
  // for BacktrackingBottomUpAstAutomata.triedBottomUpBranchs(set)
  bool compare(BacktrackingBottomUpBranch *o);
  // for BacktrackingBottomUpAstAutomata.triedBottomUpBranchs(set)
  BacktrackingBottomUpBranch *uniqueIdentificationClone() const;

  BacktrackingBottomUpBranchStatus status;
  std::list<ReducingSymbol *> reducingSymbols;
};

#endif//CYANA_AST__BACKTRACKINGBOTTOMUPBRANCH_H_
