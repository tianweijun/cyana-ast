//
// Created by tian wei jun on 2022/11/27 0027.
//

#ifndef CYANA_AST__BYTEBUFFER_H_
#define CYANA_AST__BYTEBUFFER_H_

#include <cstdint>

using byte = uint8_t;

class ByteBuffer {
 public:
  ByteBuffer(int capacity);
  ByteBuffer(int capacity, bool isBigEndian);
  ~ByteBuffer();
  int getInt();
  int getIntB();
  int getIntL();

  bool isBigEndian;
  int capacity;
  byte *buffer;
};

#endif//CYANA_AST__BYTEBUFFER_H_
