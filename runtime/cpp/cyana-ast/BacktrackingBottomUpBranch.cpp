//
// Created by tian wei jun on 2022/12/4 0004.
//

#include "BacktrackingBottomUpBranch.h"

BacktrackingBottomUpBranch::BacktrackingBottomUpBranch() : status(BacktrackingBottomUpBranchStatus::CREATED),
                                                           reducingSymbols(std::list<ReducingSymbol *>()) {
}

BacktrackingBottomUpBranch::~BacktrackingBottomUpBranch() {
  for (auto reducingSymbolsIt = reducingSymbols.begin(); reducingSymbolsIt != reducingSymbols.end(); reducingSymbolsIt++) {
    ReducingSymbol *reducingSymbol = *reducingSymbolsIt;
    delete reducingSymbol;
    reducingSymbol = 0;
  }
  reducingSymbols.clear();
}

BacktrackingBottomUpBranch *BacktrackingBottomUpBranch::clone() const {
  BacktrackingBottomUpBranch *bottomUpBranch = new BacktrackingBottomUpBranch();
  bottomUpBranch->status = this->status;
  for (auto reducingSymbolsIt = reducingSymbols.begin(); reducingSymbolsIt != reducingSymbols.end(); reducingSymbolsIt++) {
    ReducingSymbol *reducingSymbol = *reducingSymbolsIt;
    bottomUpBranch->reducingSymbols.push_back(reducingSymbol->clone());
  }
  return bottomUpBranch;
}

// for BacktrackingBottomUpAstAutomata.triedBottomUpBranchs(set)
BacktrackingBottomUpBranch *BacktrackingBottomUpBranch::uniqueIdentificationClone() const {
  BacktrackingBottomUpBranch *bottomUpBranch = new BacktrackingBottomUpBranch();
  bottomUpBranch->status = this->status;
  for (auto reducingSymbolsIt = reducingSymbols.begin(); reducingSymbolsIt != reducingSymbols.end(); reducingSymbolsIt++) {
    ReducingSymbol *reducingSymbol = *reducingSymbolsIt;
    bottomUpBranch->reducingSymbols.push_back(reducingSymbol->uniqueIdentificationClone());
  }
  return bottomUpBranch;
}

// for BacktrackingBottomUpAstAutomata.triedBottomUpBranchs(set)
bool BacktrackingBottomUpBranch::compare(BacktrackingBottomUpBranch *o) {
  if (this->status != o->status) {
    return this->status < o->status;
  }
  if (this->reducingSymbols.size() != o->reducingSymbols.size()) {
    return this->reducingSymbols.size() < o->reducingSymbols.size();
  }
  auto thisReducingSymbolsIt = this->reducingSymbols.begin();
  auto oReducingSymbolsIt = o->reducingSymbols.begin();
  while (thisReducingSymbolsIt != this->reducingSymbols.end()) {
    ReducingSymbol *thisReducingSymbol = *thisReducingSymbolsIt;
    ReducingSymbol *oReducingSymbol = *oReducingSymbolsIt;
    if (!thisReducingSymbol->equals(oReducingSymbol)) {
      return thisReducingSymbol->compare(oReducingSymbol);
    }
    thisReducingSymbolsIt++;
    oReducingSymbolsIt++;
  }
  return false;
}