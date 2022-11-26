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

class PersistentAutomataAstApplication {
 public:
  PersistentAutomataAstApplication(std::string persistentDataFilePath);

  PersistentData *persistentData;
  PersistentObject *persistentObject;
  DfaTokenAutomata *dfaTokenAutomata;
  BacktrackingBottomUpAstAutomata *astAutomata;
  void buildContext();
  Ast *buildAst(std::string sourceCodeFilePath);
};

#endif//CYANA_AST_RUNTIME__PERSISTENTAUTOMATAASTAPPLICATION_H_
