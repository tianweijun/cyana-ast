//
// Created by tian wei jun on 2022/12/4 0004.
//

#include "ReducingSymbol.h"

ReducingSymbol::ReducingSymbol() : endIndexOfToken(-1),
                                   reducedGrammar(0),
                                   astOfCurrentDfaState(0),
                                   currentDfaState(0) {
}

ReducingSymbol::~ReducingSymbol() {
  //reducedGrammar delete by PersistentData.grammars
  //currentDfaState delete by SyntaxDfa
  delete astOfCurrentDfaState;
  astOfCurrentDfaState = 0;
}

ReducingSymbol *ReducingSymbol::clone() {
  ReducingSymbol *reducingSymbol = new ReducingSymbol();
  reducingSymbol->endIndexOfToken = this->endIndexOfToken;
  reducingSymbol->reducedGrammar = this->reducedGrammar;
  reducingSymbol->currentDfaState = this->currentDfaState;
  if (0 != this->astOfCurrentDfaState) {// 归约分支栈底的ast可能为null
    reducingSymbol->astOfCurrentDfaState = this->astOfCurrentDfaState->clone();
  }
  return reducingSymbol;
}

// for BacktrackingBottomUpAstAutomata.triedBottomUpBranchs(set)
ReducingSymbol *ReducingSymbol::uniqueIdentificationClone() {
  ReducingSymbol *reducingSymbol = new ReducingSymbol();
  reducingSymbol->endIndexOfToken = this->endIndexOfToken;
  reducingSymbol->reducedGrammar = this->reducedGrammar;
  reducingSymbol->currentDfaState = this->currentDfaState;
  return reducingSymbol;
}

// for BacktrackingBottomUpAstAutomata.triedBottomUpBranchs(set)
bool ReducingSymbol::equals(ReducingSymbol *o) {
  return this->endIndexOfToken == o->endIndexOfToken && this->reducedGrammar == o->reducedGrammar && this->currentDfaState == o->currentDfaState;
}

// for BacktrackingBottomUpAstAutomata.triedBottomUpBranchs(set)
bool ReducingSymbol::compare(ReducingSymbol *o) {
  if (this->endIndexOfToken != o->endIndexOfToken) {
    return this->endIndexOfToken < o->endIndexOfToken;
  }
  if (this->reducedGrammar != o->reducedGrammar) {
    return this->reducedGrammar < o->reducedGrammar;
  }
  if (this->currentDfaState != o->currentDfaState) {
    return this->currentDfaState < o->currentDfaState;
  }
  return false;
}
