//
// Created by tian wei jun on 2022/11/23 0023.
//

#include "RuntimeAutomataAstApplication.h"
#include <list>

void RuntimeAutomataAstApplication::setContext(std::string automataFilePath) {
  persistentAutomataAstApplication = new PersistentAutomataAstApplication(automataFilePath);
}

Ast *RuntimeAutomataAstApplication::buildAst(std::string sourceCodeFilePath) {
  return persistentAutomataAstApplication->buildAst(sourceCodeFilePath);
}