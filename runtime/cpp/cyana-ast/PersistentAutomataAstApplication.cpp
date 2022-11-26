//
// Created by tian wei jun on 2022/11/23 0023.
//

#include "PersistentAutomataAstApplication.h"
#include "Logger.h"
#include <list>

using namespace logger;

PersistentAutomataAstApplication::PersistentAutomataAstApplication(std::string persistentDataFilePath) {
  persistentData = new PersistentData(persistentDataFilePath);
  buildContext();
}

void PersistentAutomataAstApplication::buildContext() {
  persistentObject = new PersistentObject(persistentData);
  dfaTokenAutomata = new DfaTokenAutomata(persistentObject->tokenDfa);
  astAutomata =
      new BacktrackingBottomUpAstAutomata(persistentObject->astDfa, persistentObject->startGrammar);
}

Ast *PersistentAutomataAstApplication::buildAst(std::string sourceCodeFilePath) {
  std::list<Token> *tokens = dfaTokenAutomata->buildToken(sourceCodeFilePath);
  Ast *ast = astAutomata->buildAst(tokens);
  Logger::info("CyanaAstApplication buildAst build successfully");
  return ast;
}
