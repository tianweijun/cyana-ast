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
  PersistentAutomataAstApplication();
  explicit PersistentAutomataAstApplication(const std::string *persistentDataFilePath);
  ~PersistentAutomataAstApplication();

  void buildContext(const std::string *persistentDataFilePath);
  const Ast *buildAst(const std::string *sourceCodeFilePath) const;

  PersistentObject *persistentObject;
  DfaTokenAutomata *dfaTokenAutomata;
  BacktrackingBottomUpAstAutomata *astAutomata;
};

#endif//CYANA_AST_RUNTIME__PERSISTENTAUTOMATAASTAPPLICATION_H_
