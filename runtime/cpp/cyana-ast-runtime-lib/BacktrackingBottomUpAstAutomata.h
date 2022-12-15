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

class BacktrackingBottomUpBranchCompare {
 public:
  bool operator()(const BacktrackingBottomUpBranch *t1, const BacktrackingBottomUpBranch *t2);
};

class BacktrackingBottomUpAstAutomata {
 public:
  BacktrackingBottomUpAstAutomata(const SyntaxDfa *astDfa, const Grammar *startGrammar);
  BacktrackingBottomUpAstAutomata(const BacktrackingBottomUpAstAutomata &backtrackingBottomUpAstAutomata) = delete;
  BacktrackingBottomUpAstAutomata(const BacktrackingBottomUpAstAutomata &&backtrackingBottomUpAstAutomata) = delete;
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
  TokenReducingSymbolInputStream tokenReducingSymbolInputStream{};
  std::list<BacktrackingBottomUpBranch *> bottomUpBranchs;
  std::set<BacktrackingBottomUpBranch *, BacktrackingBottomUpBranchCompare> triedBottomUpBranchs;

  const SyntaxDfa *astDfa;
  const Grammar *startGrammar;
  const Ast *result;
};

#endif//CYANA_AST__BACKTRACKINGBOTTOMUPASTAUTOMATA_H_
