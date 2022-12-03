//
// Created by tian wei jun on 2022/11/25 0025.
//

#include "PersistentObject.h"

PersistentObject::PersistentObject(PersistentData *persistentData) : persistentData(persistentData),
                                                                     tokenDfa(0), astDfa(0),
                                                                     startGrammar(0) {
  init();
}

PersistentObject::~PersistentObject() {
  delete tokenDfa;
  tokenDfa = 0;
  delete persistentData;
  persistentData = 0;
  //startGrammar delete by persistentData.grammars
}

void PersistentObject::init() {
  // 按文件组织顺序获得各个部分数据，每个部分获取一次
  initStringPool();
  initGrammars();
  initTokenDfa();

  persistentData->compact();
}

void PersistentObject::initTokenDfa() {
  tokenDfa = persistentData->getTokenDfaByInputStream();
}

void PersistentObject::initGrammars() {
  persistentData->getGrammarsByInputStream();
}

void PersistentObject::initStringPool() {
  persistentData->getStringPoolByInputStream();
}