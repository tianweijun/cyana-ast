//
// Created by tian wei jun on 2022/11/23 0023.
//

#ifndef CYANA_AST_RUNTIME__PERSISTENTAUTOMATAASTAPPLICATION_H_
#define CYANA_AST_RUNTIME__PERSISTENTAUTOMATAASTAPPLICATION_H_

#include "Ast.h"
#include "BacktrackingBottomUpAstAutomata.h"
#include "DfaTokenAutomata.h"
#include "PersistentData.h"
#include "PersistentObject.h"
#include <string>

class __declspec(dllimport) PersistentAutomataAstApplication {
 public:
  PersistentAutomataAstApplication();
  ~PersistentAutomataAstApplication();
  PersistentAutomataAstApplication(const std::string *persistentDataFilePath);
  void buildContext(const std::string *persistentDataFilePath);
  const Ast *buildAst(const std::string *sourceCodeFilePath);

  PersistentObject *persistentObject;
  DfaTokenAutomata *dfaTokenAutomata;
  BacktrackingBottomUpAstAutomata *astAutomata;
};

#endif//CYANA_AST_RUNTIME__PERSISTENTAUTOMATAASTAPPLICATION_H_
