//
// Created by tian wei jun on 2022/12/4 0004.
//

#include "TokenReducingSymbolInputStream.h"

TokenReducingSymbolInputStream::TokenReducingSymbolInputStream() : tokenReducingSymbols(nullptr),
                                                                   nextReadIndex(0),
                                                                   sizeOfTokenReducingSymbols(0) {
}

TokenReducingSymbolInputStream::TokenReducingSymbolInputStream(std::list<Token *> *sourceTokens) : tokenReducingSymbols(nullptr),
                                                                                                   nextReadIndex(0),
                                                                                                   sizeOfTokenReducingSymbols(0) {
  init(sourceTokens);
}

void TokenReducingSymbolInputStream::clear() {
  delete[] tokenReducingSymbols;
  tokenReducingSymbols = nullptr;
  sizeOfTokenReducingSymbols = 0;

  nextReadIndex = 0;
}

TokenReducingSymbolInputStream::~TokenReducingSymbolInputStream() {
  clear();
}

void TokenReducingSymbolInputStream::init(std::list<Token *> *sourceToken) {
  clear();
  std::list<Token *> textTokens;
  for (auto sourceTokenIt = sourceToken->begin(); sourceTokenIt != sourceToken->end();) {
    auto &token = *sourceTokenIt;
    if (token->type == TokenType::TEXT) {
      textTokens.push_back(token);
      sourceToken->erase(sourceTokenIt++);
    } else {
      sourceTokenIt++;
    }
  }
  sizeOfTokenReducingSymbols = textTokens.size();
  tokenReducingSymbols = new Token *[sizeOfTokenReducingSymbols];
  int indexOfTokenReducingSymbols = 0;
  for (auto token : textTokens) {
    tokenReducingSymbols[indexOfTokenReducingSymbols++] = token;
  }
}

Token *TokenReducingSymbolInputStream::read() {
  if (hasNext()) {
    return tokenReducingSymbols[nextReadIndex++];
  }
  return nullptr;
}

bool TokenReducingSymbolInputStream::hasNext() const {
  return nextReadIndex >= 0 && nextReadIndex < sizeOfTokenReducingSymbols;
}

bool TokenReducingSymbolInputStream::hasReadAll() const {
  return nextReadIndex >= sizeOfTokenReducingSymbols;
}
