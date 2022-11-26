//
// Created by tian wei jun on 2022/11/25 0025.
//

#ifndef CYANA_AST__DFATOKENAUTOMATA_H_
#define CYANA_AST__DFATOKENAUTOMATA_H_
#include "Token.h"
#include "TokenDfa.h"
#include <list>
#include <string>

class DfaTokenAutomata {
 public:
  DfaTokenAutomata(TokenDfa *tokenDfa);
  std::list<Token> *buildToken(std::string sourceFilePath);

  TokenDfa *tokenDfa;
};

#endif//CYANA_AST__DFATOKENAUTOMATA_H_
