//
// Created by tian wei jun on 2022/12/4 0004.
//

#include "TokenReducingSymbolInputStream.h"

TokenReducingSymbolInputStream::TokenReducingSymbolInputStream(std::list<Token *> *sourceTokens) : tokenReducingSymbols(0),
                                                                                                   nextReadIndex(0),
                                                                                                   sizeOfTokenReducingSymbols(0) {
  setTokenReducingSymbols(sourceTokens);
}

TokenReducingSymbolInputStream::~TokenReducingSymbolInputStream() {
  for (int i = 0; i < sizeOfTokenReducingSymbols; i++) {
    Token *token = tokenReducingSymbols[i];
    delete token;
    token = 0;
  }
  delete[] tokenReducingSymbols;
  tokenReducingSymbols = 0;
}

void TokenReducingSymbolInputStream::setTokenReducingSymbols(std::list<Token *> *sourceToken) {
  std::list<Token *> textTokens;
  for (auto sourceTokenIt = sourceToken->begin(); sourceTokenIt != sourceToken->end(); sourceTokenIt++) {
    Token *token = *sourceTokenIt;
    if (token->type == TokenType::TEXT) {
      textTokens.push_back(token);
    }
  }
  sizeOfTokenReducingSymbols = textTokens.size();
  tokenReducingSymbols = new Token *[sizeOfTokenReducingSymbols];
  int indexOfTokenReducingSymbols = 0;
  for (auto tokenIt = textTokens.begin(); tokenIt != textTokens.end(); tokenIt++) {
    Token *token = *tokenIt;
    tokenReducingSymbols[indexOfTokenReducingSymbols++] = const_cast<Token *>(token->clone());
  }
}

Token *TokenReducingSymbolInputStream::read() {
  if (hasNext()) {
    return tokenReducingSymbols[nextReadIndex++];
  }
  return 0;
}

bool TokenReducingSymbolInputStream::hasNext() {
  return nextReadIndex >= 0 && nextReadIndex < sizeOfTokenReducingSymbols;
}

bool TokenReducingSymbolInputStream::hasReadAll() {
  return nextReadIndex >= sizeOfTokenReducingSymbols;
}
