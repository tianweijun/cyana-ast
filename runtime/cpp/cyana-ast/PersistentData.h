//
// Created by tian wei jun on 2022/11/24 0024.
//

#ifndef CYANA_AST_RUNTIME__PERSISTENTDATA_H_
#define CYANA_AST_RUNTIME__PERSISTENTDATA_H_
#include "ByteBuffer.h"
#include <fstream>
#include <string>

//using byte = char;

class PersistentData {
 public:
  PersistentData(std::string automataFilePath);
  void init(std::string automataFilePath);
  ~PersistentData();

  std::ifstream inputStream;
  ByteBuffer **stringPool;
  int sizeOfStringPool;
  ByteBuffer intByteBuffer;

  ByteBuffer *readByteString(int countOfStringBytes);
  ByteBuffer **getStringPoolByInputStream();
  void doRead(byte bytes[], int offset, int length);
  int readInt();
  void compact();
};

#endif//CYANA_AST_RUNTIME__PERSISTENTDATA_H_
