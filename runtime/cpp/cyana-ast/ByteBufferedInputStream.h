//
// Created by tian wei jun on 2022/12/2 0002.
//

#ifndef CYANA_AST__BYTEBUFFEREDINPUTSTREAM_H_
#define CYANA_AST__BYTEBUFFEREDINPUTSTREAM_H_

#include <fstream>
using byte = uint8_t;

class ByteBufferedInputStream {
 public:
  ByteBufferedInputStream();
  ~ByteBufferedInputStream();

 public:
  void init(const std::string *sourceFilePath);
  int read();
  bool fill();
  bool fillByExpansion();
  bool fillByNoMark();
  void skip(int skipSteps);
  void reset();
  void mark();
  int doRead();

 public:
  int nextReadIndex;

 private:
  int standardBufferCapacity;
  int eof;
  int nextPos;
  int count;
  int markPos;
  byte *buffer;
  int sizeOfBuffer;
  std::ifstream *byteInputStream;
};

#endif//CYANA_AST__BYTEBUFFEREDINPUTSTREAM_H_