//
// Created by tian wei jun on 2022/11/23 0023.
//

#include "PersistentAutomataAstApplication.h"
#include <iostream>
#include <list>

PersistentAutomataAstApplication::PersistentAutomataAstApplication() : persistentObject(nullptr),
                                                                       dfaTokenAutomata(nullptr),
                                                                       astAutomata(nullptr) {
}

PersistentAutomataAstApplication::PersistentAutomataAstApplication(const std::string *persistentDataFilePath) : PersistentAutomataAstApplication() {
  buildContext(persistentDataFilePath);
}
PersistentAutomataAstApplication::~PersistentAutomataAstApplication() {
  delete dfaTokenAutomata;
  dfaTokenAutomata = nullptr;
  delete astAutomata;
  astAutomata = nullptr;
  delete persistentObject;
  persistentObject = nullptr;
}

void PersistentAutomataAstApplication::buildContext(const std::string *persistentDataFilePath) {
  auto *persistentData = new PersistentData(persistentDataFilePath);
  persistentObject = new PersistentObject(persistentData);
  dfaTokenAutomata = new DfaTokenAutomata(persistentObject->tokenDfa);
  astAutomata =
      new BacktrackingBottomUpAstAutomata(persistentObject->astDfa, persistentObject->startGrammar);
}

const Ast *PersistentAutomataAstApplication::buildAst(const std::string *sourceCodeFilePath) const {
  std::list<Token *> *tokens = dfaTokenAutomata->buildToken(sourceCodeFilePath);
  const Ast *ast = astAutomata->buildAst(tokens);
  for (auto token : *tokens) {
    delete token;
  }
  delete tokens;
  tokens = nullptr;
  return ast;
}
