//
// Created by tian wei jun on 2022/11/24 0024.
//

#include "PersistentData.h"
#include "CyanaAstRuntimeException.h"
#include <cstdlib>
#include <iostream>
#include <sstream>

PersistentData::PersistentData(std::string automataFilePath) : inputStream(std::ifstream()),
                                                               intByteBuffer(ByteBuffer(4, true)) {
  stringPool = 0;
  init(automataFilePath);
}

void PersistentData::init(std::string automataFilePath) {
  inputStream.open(automataFilePath, std::ios::in | std::ios::binary);

  if (!inputStream.is_open()) {
    std::stringstream exceptionInfo;
    exceptionInfo << "open automata File error,path:'" << automataFilePath << "'";
    throw CyanaAstRuntimeException(exceptionInfo.str());
  }
}

PersistentData::~PersistentData() {
  inputStream.close();
  free(stringPool);
  stringPool = 0;
}

ByteBuffer **PersistentData::getStringPoolByInputStream() {
  int sizeOfString = readInt();
  ByteBuffer **strings = (ByteBuffer **) malloc(sizeOfString * sizeof(ByteBuffer *));
  for (int indexOfStrings = 0; indexOfStrings < sizeOfString; indexOfStrings++) {
    int countOfStringBytes = readInt();
    ByteBuffer *str = readByteString(countOfStringBytes);
    strings[indexOfStrings] = str;
  }
  this->stringPool = strings;
  this->sizeOfStringPool = sizeOfString;
  return strings;
}

ByteBuffer *PersistentData::readByteString(int countOfStringBytes) {
  ByteBuffer *byteBuffer = new ByteBuffer(countOfStringBytes);
  doRead(byteBuffer->buffer, 0, countOfStringBytes);
  return byteBuffer;
}

int PersistentData::readInt() {
  doRead(intByteBuffer.buffer, 0, intByteBuffer.capacity);
  return intByteBuffer.getInt();
}

void PersistentData::doRead(byte bytes[], int offset, int length) {
  char *base = (char *) (&(bytes[offset]));
  inputStream.read(base, length);
}

void PersistentData::compact() {
  inputStream.close();
}
