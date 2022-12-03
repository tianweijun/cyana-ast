//
// Created by tian wei jun on 2022/11/25 0025.
//

#ifndef CYANA_AST__BACKTRACKINGBOTTOMUPASTAUTOMATA_H_
#define CYANA_AST__BACKTRACKINGBOTTOMUPASTAUTOMATA_H_

#include "Ast.h"
#include "Grammar.h"
#include "SyntaxDfa.h"
#include "Token.h"
#include <list>

class BacktrackingBottomUpAstAutomata {
 public:
  BacktrackingBottomUpAstAutomata(SyntaxDfa *astDfa, Grammar *startGrammar);
  Ast *buildAst(std::list<Token *> *sourceTokens);
  SyntaxDfa *astDfa;
  Grammar *startGrammar;
};

#endif//CYANA_AST__BACKTRACKINGBOTTOMUPASTAUTOMATA_H_
