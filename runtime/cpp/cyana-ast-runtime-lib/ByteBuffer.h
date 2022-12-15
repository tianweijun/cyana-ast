//
// Created by tian wei jun on 2022/11/27 0027.
//

#ifndef CYANA_AST__BYTEBUFFER_H_
#define CYANA_AST__BYTEBUFFER_H_

#include <cstdint>

using byte = uint8_t;

class ByteBuffer {
 public:
  ByteBuffer();
  explicit ByteBuffer(int capacity);
  ByteBuffer(int capacity, bool isBigEndian);
  ByteBuffer(const ByteBuffer &byteBuffer) = delete;
  ByteBuffer(const ByteBuffer &&byteBuffer) = delete;
  ~ByteBuffer();

  void setPosition(int pos);
  int length() const;
  void append(byte b);
  void clear();
  int getInt() const;
  int getIntB() const;
  int getIntL() const;

  bool isBigEndian;
  int capacity;
  int position;
  byte *buffer;

 private:
  void extendBuffer();

 private:
  const static int standardBufferCapacity;
};

#endif//CYANA_AST__BYTEBUFFER_H_
