//
// Created by tian wei jun on 2022/12/2 0002.
//

#include "ByteBufferedInputStream.h"
#include "CyanaAstRuntimeException.h"

int ByteBufferedInputStream::standardBufferCapacity = 256;

ByteBufferedInputStream::ByteBufferedInputStream() : nextReadIndex(0), eof(-1), nextPos(0),
                                                     count(0), markPos(-1), sizeOfBuffer(0),
                                                     buffer(nullptr), byteInputStream(nullptr) {
}

ByteBufferedInputStream::~ByteBufferedInputStream() {
  delete[] buffer;
  buffer = nullptr;

  if (byteInputStream) {
    byteInputStream->close();
    delete byteInputStream;
    byteInputStream = nullptr;
  }
}

int ByteBufferedInputStream::read() {
  int readedByte = eof;
  if (nextPos < count) {
    readedByte = buffer[nextPos++];
    ++nextReadIndex;
  } else {
    if (fill()) {
      readedByte = read();
    }
  }
  return readedByte;
}

/*
 * 只有当读完缓冲后才能调用，其他时候不能调用，只能在read方法中调用.
 */
bool ByteBufferedInputStream::fill() {
  if (markPos == eof) {// 重新填入
    return fillByNoMark();
  } else if (markPos == 0) {// 扩容
    return fillByExpansion();
  } else {// mark>0 && mark<=count 移动
    for (int indexOfBuffer = markPos; indexOfBuffer < count; indexOfBuffer++) {
      buffer[indexOfBuffer - markPos] = buffer[indexOfBuffer];
    }
    int oldCount = count - markPos;
    int newCount = oldCount;
    for (int indexOfBuffer = oldCount; indexOfBuffer < sizeOfBuffer; indexOfBuffer++) {
      int read = doRead();
      if (eof == read) {
        break;
      }
      buffer[indexOfBuffer] = (byte) read;
      ++newCount;
    }
    this->count = newCount;
    this->nextPos -= markPos;
    this->markPos = 0;
    return newCount > oldCount;
  }
}

bool ByteBufferedInputStream::fillByExpansion() {
  int nsz = count + standardBufferCapacity;
  byte *nBuffer = new byte[nsz];
  for (int i = 0; i < count; i++) {
    nBuffer[i] = buffer[i];
  }
  int newCount = count;
  for (int indexOfNewBuffer = count; indexOfNewBuffer < nsz; indexOfNewBuffer++) {
    int read = doRead();
    if (eof == read) {
      break;
    }
    nBuffer[indexOfNewBuffer] = (byte) read;
    ++newCount;
  }
  delete[] this->buffer;
  this->buffer = nBuffer;
  this->count = newCount;
  this->sizeOfBuffer = nsz;
  return newCount > count;
}

bool ByteBufferedInputStream::fillByNoMark() {
  int read = doRead();
  if (eof == read) {
    return false;
  }
  markPos = eof;
  nextPos = 0;
  count = 0;

  int indexOfFill = 0;
  buffer[indexOfFill++] = (byte) read;
  ++count;

  while (indexOfFill < sizeOfBuffer) {
    read = doRead();
    if (eof == read) {
      break;
    }
    buffer[indexOfFill++] = (byte) read;
    ++count;
  }
  return true;
}

void ByteBufferedInputStream::skip(int skipSteps) {
  for (int skipTimes = 0; skipTimes < skipSteps; skipTimes++) {
    read();
  }
}

void ByteBufferedInputStream::reset() {
  if (markPos == eof) {
    return;
  }
  nextReadIndex = nextReadIndex - (nextPos - markPos);
  nextPos = markPos;
  markPos = eof;
}

void ByteBufferedInputStream::mark() {
  this->markPos = nextPos;
}

int ByteBufferedInputStream::doRead() {
  int read = byteInputStream->get();
  if (byteInputStream->fail()) {
    read = -1;
  }
  return read;
}

void ByteBufferedInputStream::init(const std::string *sourceFilePath) {
  nextReadIndex = 0;
  eof = -1;
  nextPos = 0;
  count = 0;
  markPos = -1;
  if (!buffer) {
    buffer = new byte[standardBufferCapacity];
    sizeOfBuffer = standardBufferCapacity;
  }

  if (!byteInputStream) {
    byteInputStream = new std::ifstream();
  } else {
    byteInputStream->close();
    byteInputStream->clear();
  }
  byteInputStream->open(*sourceFilePath, std::ios::in | std::ios::binary);
  if (!byteInputStream->is_open()) {
    throw CyanaAstRuntimeException("open source File error,path:'" + *sourceFilePath + "'");
  }
}