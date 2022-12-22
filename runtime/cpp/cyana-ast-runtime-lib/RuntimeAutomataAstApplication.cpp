//
// Created by tian wei jun on 2022/11/23 0023.
//

#include "RuntimeAutomataAstApplication.h"
#include "CyanaAstRuntimeException.h"
#include <list>

RuntimeAutomataAstApplication::RuntimeAutomataAstApplication() : persistentAutomataAstApplication(nullptr) {
}

RuntimeAutomataAstApplication::~RuntimeAutomataAstApplication() {
  delete persistentAutomataAstApplication;
  persistentAutomataAstApplication = nullptr;
}

void RuntimeAutomataAstApplication::setContext(const std::string *automataFilePath) {
  HandlerExceptionResolver::clearExceptions();
  persistentAutomataAstApplication = new PersistentAutomataAstApplication(automataFilePath);
}

const Ast *RuntimeAutomataAstApplication::buildAst(const std::string *sourceCodeFilePath) {
  HandlerExceptionResolver::clearExceptions();
  return persistentAutomataAstApplication->buildAst(sourceCodeFilePath);
}