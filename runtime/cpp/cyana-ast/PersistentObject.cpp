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
  delete persistentData;
  persistentData = 0;
}

void PersistentObject::init() {
  // 按文件组织顺序获得各个部分数据，每个部分获取一次
  initStringPool();

  persistentData->compact();
}

void PersistentObject::initStringPool() {
  persistentData->getStringPoolByInputStream();
}