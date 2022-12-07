//
// Created by tian wei jun on 2022/11/25 0025.
//

#ifndef CYANA_AST__BACKTRACKINGBOTTOMUPASTAUTOMATA_H_
#define CYANA_AST__BACKTRACKINGBOTTOMUPASTAUTOMATA_H_

#include "Ast.h"
#include "BacktrackingBottomUpBranch.h"
#include "Grammar.h"
#include "SyntaxDfa.h"
#include "Token.h"
#include "TokenReducingSymbolInputStream.h"
#include <list>
#include <set>

class __declspec(dllimport) BacktrackingBottomUpBranchCompare {
 public:
  bool operator()(BacktrackingBottomUpBranch *t1, BacktrackingBottomUpBranch *t2) {
    return t1->compare(t2);
  }
};

class __declspec(dllimport) BacktrackingBottomUpAstAutomata {
 public:
  BacktrackingBottomUpAstAutomata(SyntaxDfa *astDfa, Grammar *startGrammar);
  ~BacktrackingBottomUpAstAutomata();

  const Ast *buildAst(std::list<Token *> *sourceTokens);

 private:
  void init(std::list<Token *> *sourceTokens);
  ReducingSymbol *getConnectedSignOfStartGrammarReducingSymbol();
  void addNewBacktrackingBottomUpBranch(BacktrackingBottomUpBranch *newBacktrackingBottomUpBranch);
  void consumeBottomUpBranch();
  void closeBottomUpBranch(BacktrackingBottomUpBranch *bottomUpBranch);
  void shiftBottomUpBranch(BacktrackingBottomUpBranch *bottomUpBranch);
  void reduceBottomUpBranch(BacktrackingBottomUpBranch *bottomUpBranch);
  void doReduce(BacktrackingBottomUpBranch *bottomUpBranch, ProductionRule *closingProductionRule);
  void clear();

 private:
  TokenReducingSymbolInputStream *tokenReducingSymbolInputStream;
  std::list<BacktrackingBottomUpBranch *> bottomUpBranchs;
  std::set<BacktrackingBottomUpBranch *, BacktrackingBottomUpBranchCompare> triedBottomUpBranchs;

  SyntaxDfa *astDfa;
  Grammar *startGrammar;
  const Ast *result;
};

#endif//CYANA_AST__BACKTRACKINGBOTTOMUPASTAUTOMATA_H_
