//
// Created by tian wei jun on 2022/11/24 0024.
//

#ifndef CYANA_AST_RUNTIME__PERSISTENTDATA_H_
#define CYANA_AST_RUNTIME__PERSISTENTDATA_H_
#include "ByteBuffer.h"
#include "Grammar.h"
#include "TokenDfa.h"
#include "TokenDfaState.h"
#include <fstream>
#include <string>

class PersistentData {
 public:
  PersistentData(std::string automataFilePath);
  void init(std::string automataFilePath);
  ~PersistentData();

  std::ifstream inputStream;
  std::string **stringPool;
  int sizeOfStringPool;
  ByteBuffer intByteBuffer;
  Grammar **grammars;
  int sizeOfGramamr;

  TokenDfa *getTokenDfaByInputStream();
  Grammar **getGrammarsByInputStream();
  std::string *readByteString(int countOfStringBytes);
  std::string **getStringPoolByInputStream();
  void doRead(byte bytes[], int offset, int length);
  int readInt();
  void compact();
};

#endif//CYANA_AST_RUNTIME__PERSISTENTDATA_H_
