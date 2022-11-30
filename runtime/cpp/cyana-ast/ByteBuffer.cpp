//
// Created by tian wei jun on 2022/11/27 0027.
//

#include "ByteBuffer.h"
#include <cstdlib>

ByteBuffer::ByteBuffer(int capacity) : capacity(capacity), isBigEndian(true) {
  buffer = (byte *) malloc(capacity * sizeof(byte));
}

ByteBuffer::ByteBuffer(int capacity, bool isBigEndian) : capacity(capacity), isBigEndian(isBigEndian) {
  buffer = (byte *) malloc(capacity * sizeof(byte));
}

ByteBuffer::~ByteBuffer() {
  free(buffer);
  buffer = 0;
}

int ByteBuffer::getInt() {
  return isBigEndian ? getIntB() : getIntL();
}

int ByteBuffer::getIntB() {
  int base = 0;
  int value = 0;
  for (int i = capacity - 1; i >= 0; i--) {
    byte tmp = buffer[i];
    value = value | tmp << base;
    base += 8;
  }
  return value;
}

int ByteBuffer::getIntL() {
  int base = 0;
  int value = 0;
  for (int i = 0; i < capacity; i++) {
    byte tmp = buffer[i];
    value = value | tmp << base;
    base += 8;
  }
  return value;
}