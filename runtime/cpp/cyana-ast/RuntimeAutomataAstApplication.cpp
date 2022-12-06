//
// Created by tian wei jun on 2022/11/23 0023.
//

#include "RuntimeAutomataAstApplication.h"
#include <list>

RuntimeAutomataAstApplication::RuntimeAutomataAstApplication() : persistentAutomataAstApplication(0) {
}

RuntimeAutomataAstApplication::~RuntimeAutomataAstApplication() {
  delete persistentAutomataAstApplication;
  persistentAutomataAstApplication = 0;
}

void RuntimeAutomataAstApplication::setContext(const std::string *automataFilePath) {
  persistentAutomataAstApplication = new PersistentAutomataAstApplication(automataFilePath);
}

const Ast *RuntimeAutomataAstApplication::buildAst(const std::string *sourceCodeFilePath) {
  return persistentAutomataAstApplication->buildAst(sourceCodeFilePath);
}