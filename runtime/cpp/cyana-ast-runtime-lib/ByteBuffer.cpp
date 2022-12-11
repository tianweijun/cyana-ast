//
// Created by tian wei jun on 2022/11/27 0027.
//

#include "ByteBuffer.h"

const int ByteBuffer::standardBufferCapacity = 256;

ByteBuffer::ByteBuffer() : capacity(standardBufferCapacity), isBigEndian(true), position(0) {
  buffer = new byte[standardBufferCapacity];
}

ByteBuffer::ByteBuffer(int capacity) : capacity(capacity), isBigEndian(true), position(0) {
  buffer = new byte[capacity];
}

ByteBuffer::ByteBuffer(int capacity, bool isBigEndian) : capacity(capacity),
                                                         isBigEndian(isBigEndian), position(0) {
  buffer = new byte[capacity];
}

ByteBuffer::~ByteBuffer() {
  delete[] buffer;
  buffer = nullptr;
}

void ByteBuffer::setPosition(int pos) {
  position = pos;
}

int ByteBuffer::length() const {
  return position;
}

void ByteBuffer::append(byte b) {
  if (position >= capacity) {
    extendBuffer();
  }
  buffer[position++] = b;
}

void ByteBuffer::extendBuffer() {
  int newCapacity = capacity + standardBufferCapacity;
  byte *newBuffer = new byte[newCapacity];
  for (int i = 0; i < capacity; i++) {
    newBuffer[i] = buffer[i];
  }
  delete[] buffer;
  buffer = newBuffer;
  capacity = newCapacity;
}

void ByteBuffer::clear() {
  position = 0;
}

int ByteBuffer::getInt() const {
  return isBigEndian ? getIntB() : getIntL();
}

int ByteBuffer::getIntB() const {
  int base = 0;
  int value = 0;
  for (int i = capacity - 1; i >= 0; i--) {
    byte tmp = buffer[i];
    value = value | tmp << base;
    base += 8;
  }
  return value;
}

int ByteBuffer::getIntL() const {
  int base = 0;
  int value = 0;
  for (int i = 0; i < capacity; i++) {
    byte tmp = buffer[i];
    value = value | tmp << base;
    base += 8;
  }
  return value;
}