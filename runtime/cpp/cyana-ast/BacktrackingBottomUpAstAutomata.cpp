//
// Created by tian wei jun on 2022/11/25 0025.
//

#include "BacktrackingBottomUpAstAutomata.h"

BacktrackingBottomUpAstAutomata::BacktrackingBottomUpAstAutomata(SyntaxDfa *astDfa, Grammar *startGrammar)
    : astDfa(astDfa), startGrammar(startGrammar) {
}

Ast *BacktrackingBottomUpAstAutomata::buildAst(std::list<Token> *sourceTokens) {
  return 0;
}