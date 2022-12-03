//
// Created by tian wei jun on 2022/11/23 0023.
//

#ifndef CYANA_AST_RUNTIME__RUNTIMEAUTOMATAASTAPPLICATION_H_
#define CYANA_AST_RUNTIME__RUNTIMEAUTOMATAASTAPPLICATION_H_
#include "Ast.h"
#include "PersistentAutomataAstApplication.h"
#include <string>

class RuntimeAutomataAstApplication {
 public:
  RuntimeAutomataAstApplication();
  ~RuntimeAutomataAstApplication();

  void setContext(const std::string automataFilePath);
  Ast *buildAst(const std::string sourceCodeFilePath);

  PersistentAutomataAstApplication *persistentAutomataAstApplication;
};

#endif//CYANA_AST_RUNTIME__RUNTIMEAUTOMATAASTAPPLICATION_H_
