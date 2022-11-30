//
// Created by tian wei jun on 2022/11/23 0023.
//

#include "PersistentAutomataAstApplication.h"
#include "Logger.h"
#include <list>

using namespace logger;

PersistentAutomataAstApplication::PersistentAutomataAstApplication() : persistentObject(0),
                                                                       dfaTokenAutomata(0),
                                                                       astAutomata(0) {
}
PersistentAutomataAstApplication::~PersistentAutomataAstApplication() {
  delete dfaTokenAutomata;
  dfaTokenAutomata = 0;
  delete astAutomata;
  astAutomata = 0;
  delete persistentObject;
  persistentObject = 0;
}

PersistentAutomataAstApplication::PersistentAutomataAstApplication(std::string persistentDataFilePath) {
  buildContext(persistentDataFilePath);
}

void PersistentAutomataAstApplication::buildContext(std::string persistentDataFilePath) {
  PersistentData *persistentData = new PersistentData(persistentDataFilePath);
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