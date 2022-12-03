//
// Created by tian wei jun on 2022/11/24 0024.
//

#include "PersistentData.h"
#include "CyanaAstRuntimeException.h"
#include <sstream>

PersistentData::PersistentData(std::string automataFilePath) : inputStream(std::ifstream()),
                                                               intByteBuffer(ByteBuffer(4, true)),
                                                               stringPool(0),
                                                               sizeOfStringPool(0), sizeOfGramamr(0) {
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

  for (int i = 0; i < sizeOfGramamr; i++) {
    Grammar *grammar = grammars[i];
    delete grammar;
    grammar = 0;
  }
  delete grammars;
  grammars = 0;

  if (stringPool != 0) {
    for (int i = 0; i < sizeOfStringPool; i++) {
      std::string *string = stringPool[i];
      delete string;
      string = 0;
    }
    delete stringPool;
    stringPool = 0;
  }
}

TokenDfa *PersistentData::getTokenDfaByInputStream() {
  int sizeOfTokenDfaStates = readInt();
  TokenDfaState **tokenDfaStates = new TokenDfaState *[sizeOfTokenDfaStates];
  for (int indexOfTokenDfaStates = 0;
       indexOfTokenDfaStates < sizeOfTokenDfaStates;
       indexOfTokenDfaStates++) {
    tokenDfaStates[indexOfTokenDfaStates] = new TokenDfaState();
  }
  // countOfTokenDfaStates-(type-weight-terminal-countOfEdges-[ch,dest]{countOfEdges})
  for (int indexOfTokenDfaStates = 0;
       indexOfTokenDfaStates < sizeOfTokenDfaStates;
       indexOfTokenDfaStates++) {
    TokenDfaState *tokenDfaState = tokenDfaStates[indexOfTokenDfaStates];
    tokenDfaState->type = readInt();
    tokenDfaState->weight = readInt();
    int intOfTerminal = readInt();
    if (intOfTerminal >= 0) {
      tokenDfaState->terminal = grammars[intOfTerminal];
    }
    int sizeOfEdges = readInt();
    for (int indexOfEdges = 0; indexOfEdges < sizeOfEdges; indexOfEdges++) {
      int ch = readInt();
      TokenDfaState *chToState = tokenDfaStates[readInt()];
      std::pair<byte, TokenDfaState *> keyValue(ch, chToState);
      tokenDfaState->edges.insert(keyValue);
    }
  }

  TokenDfa *tokenDfa = new TokenDfa();
  tokenDfa->sizeOfStates = sizeOfTokenDfaStates;
  tokenDfa->states = tokenDfaStates;
  tokenDfa->start = tokenDfaStates[0];
  return tokenDfa;
}

Grammar **PersistentData::getGrammarsByInputStream() {
  int sizeOfGramamr = readInt();
  Grammar **grammars = new Grammar *[sizeOfGramamr];

  for (int indexOfGrammars = 0; indexOfGrammars < sizeOfGramamr; indexOfGrammars++) {
    std::string *name = stringPool[readInt()];
    Grammar *grammar = new Grammar(name);
    grammar->type = GrammarType(readInt());
    grammar->action = GrammarAction(readInt());
    grammars[indexOfGrammars] = grammar;
  }
  this->sizeOfGramamr = sizeOfGramamr;
  this->grammars = grammars;
  return grammars;
}

std::string **PersistentData::getStringPoolByInputStream() {
  int sizeOfString = readInt();
  std::string **strings = new std::string *[sizeOfString];
  for (int indexOfStrings = 0; indexOfStrings < sizeOfString; indexOfStrings++) {
    int countOfStringBytes = readInt();
    std::string *str = readByteString(countOfStringBytes);
    strings[indexOfStrings] = str;
  }
  this->stringPool = strings;
  this->sizeOfStringPool = sizeOfString;
  return strings;
}

std::string *PersistentData::readByteString(int countOfStringBytes) {
  byte *byteBuffer = new byte[countOfStringBytes];
  doRead(byteBuffer, 0, countOfStringBytes);
  return new std::string((char *) byteBuffer, countOfStringBytes);
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
