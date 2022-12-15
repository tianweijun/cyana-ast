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
  explicit PersistentObject(PersistentData *persistentData);
  PersistentObject(const PersistentObject &persistentObject) = delete;
  PersistentObject(const PersistentObject &&persistentObject) = delete;
  ~PersistentObject();

  void init();

 private:
  void initStringPool() const;
  void initGrammars() const;
  void initTokenDfa();
  void initStartGrammar();
  void initProductionRules() const;
  void initAstDfa();

 private:
  PersistentData *persistentData;

 public:
  const TokenDfa *tokenDfa;
  const SyntaxDfa *astDfa;
  const Grammar *startGrammar;
};

#endif//CYANA_AST__PERSISTENTOBJECT_H_
