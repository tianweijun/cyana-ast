//
// Created by tian wei jun on 2022/11/25 0025.
//

#ifndef CYANA_AST__PERSISTENTOBJECT_H_
#define CYANA_AST__PERSISTENTOBJECT_H_

#include "Grammar.h"
#include "PersistentData.h"
#include "SyntaxDfa.h"
#include "TokenDfa.h"

class PersistentObject {
 public:
  PersistentObject(PersistentData *persistentData);
  ~PersistentObject();

  void init();
  void initStringPool();

  PersistentData *persistentData;
  TokenDfa *tokenDfa;
  SyntaxDfa *astDfa;
  Grammar *startGrammar;
};

#endif//CYANA_AST__PERSISTENTOBJECT_H_
