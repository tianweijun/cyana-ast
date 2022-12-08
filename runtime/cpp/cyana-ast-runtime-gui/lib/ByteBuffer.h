//
// Created by tian wei jun on 2022/11/27 0027.
//

#ifndef CYANA_AST__BYTEBUFFER_H_
#define CYANA_AST__BYTEBUFFER_H_

#include <cstdint>

using byte = uint8_t;

class __declspec(dllimport) ByteBuffer {
 public:
  ByteBuffer();
  ByteBuffer(int capacity);
  ByteBuffer(int capacity, bool isBigEndian);
  ~ByteBuffer();

  void setPosition(int pos);
  int length();
  void append(byte b);
  void clear();
  int getInt();
  int getIntB();
  int getIntL();

  bool isBigEndian;
  int capacity;
  int position;
  byte *buffer;

 private:
  void extendBuffer();

 private:
  static int standardBufferCapacity;
};

#endif//CYANA_AST__BYTEBUFFER_H_
