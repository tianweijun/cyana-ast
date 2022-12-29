//
// Created by tian wei jun on 2022/11/24 0024.
//

#include "PersistentData.h"
#include "CyanaAstRuntimeException.h"
#include "SyntaxDfaState.h"

PersistentData::PersistentData(const std::string *automataFilePath) : stringPool(nullptr), grammars(nullptr),
                                                                      sizeOfStringPool(0), sizeOfGramamrs(0),
                                                                      productionRules(nullptr), sizeOfProductionRules(0) {
  init(automataFilePath);
}

void PersistentData::init(const std::string *automataFilePath) {
  inputStream.open(*automataFilePath, std::ios::in | std::ios::binary);

  if (!inputStream.is_open()) {
    CyanaAstRuntimeExceptionResolver::throwException(
        new CyanaAstRuntimeException(CyanaAstRuntimeExceptionCode::IO_ERROR,
                                     "open automata File error,path:'" + *automataFilePath + "'"));
  }
}

PersistentData::~PersistentData() {
  inputStream.close();

  for (int i = 0; i < sizeOfProductionRules; i++) {
    ProductionRule *productionRule = productionRules[i];
    delete productionRule;
    productionRule = nullptr;
  }
  delete[] productionRules;
  productionRules = nullptr;

  for (int i = 0; i < sizeOfGramamrs; i++) {
    Grammar *grammar = grammars[i];
    delete grammar;
    grammar = nullptr;
  }
  delete[] grammars;
  grammars = nullptr;

  if (stringPool) {
    for (int i = 0; i < sizeOfStringPool; i++) {
      std::string *string = stringPool[i];
      delete string;
      string = nullptr;
    }
    delete[] stringPool;
    stringPool = nullptr;
  }
}

void PersistentData::getProductionRulesByInputStream() {
  int countOfProductionRules = readInt();
  auto **tmpProductionRules = new ProductionRule *[countOfProductionRules];
  for (int indexOfProductionRules = 0;
       indexOfProductionRules < countOfProductionRules;
       indexOfProductionRules++) {
    tmpProductionRules[indexOfProductionRules] = new ProductionRule();
  }
  this->productionRules = tmpProductionRules;

  for (int indexOfProductionRules = 0;
       indexOfProductionRules < countOfProductionRules;
       indexOfProductionRules++) {
    ProductionRule *productionRule = tmpProductionRules[indexOfProductionRules];
    productionRule->grammar = grammars[readInt()];
    int indexOfAliasInStringPool = readInt();
    if (indexOfAliasInStringPool >= 0) {
      productionRule->alias = stringPool[indexOfAliasInStringPool];
    }
    productionRule->reversedDfa = getSyntaxDfaByInputStream();
  }
}

SyntaxDfa *PersistentData::getSyntaxDfaByInputStream() {
  int sizeOfSyntaxDfaStates = readInt();
  auto **syntaxDfaStates = new SyntaxDfaState *[sizeOfSyntaxDfaStates];
  for (int indexOfSyntaxDfaStates = 0;
       indexOfSyntaxDfaStates < sizeOfSyntaxDfaStates;
       indexOfSyntaxDfaStates++) {
    syntaxDfaStates[indexOfSyntaxDfaStates] = new SyntaxDfaState();
  }
  // countOfSyntaxDfaStates-(type-countOfEdges-[ch,dest]{countOfEdges}-countOfProductions-productions)
  for (int indexOfSyntaxDfaStates = 0;
       indexOfSyntaxDfaStates < sizeOfSyntaxDfaStates;
       indexOfSyntaxDfaStates++) {
    SyntaxDfaState *syntaxDfaState = syntaxDfaStates[indexOfSyntaxDfaStates];
    syntaxDfaState->type = readInt();
    int sizeOfEdges = readInt();
    for (int indexOfEdges = 0; indexOfEdges < sizeOfEdges; indexOfEdges++) {
      Grammar *ch = grammars[readInt()];
      SyntaxDfaState *chToState = syntaxDfaStates[readInt()];
      std::pair<const Grammar *, SyntaxDfaState *> keyValue(ch, chToState);
      syntaxDfaState->edges.insert(keyValue);
    }
    int sizeOfProductions = readInt();
    for (int indexOfProductions = 0;
         indexOfProductions < sizeOfProductions;
         indexOfProductions++) {
      syntaxDfaState->closingProductionRules.push_back(productionRules[readInt()]);
    }
  }
  auto *syntaxDfa = new SyntaxDfa(syntaxDfaStates[0],
                                  (const SyntaxDfaState **) syntaxDfaStates,
                                  sizeOfSyntaxDfaStates);
  return syntaxDfa;
}

Grammar *PersistentData::getStartGrammarByInputStream() {
  int indexOfStartGrammar = readInt();
  return grammars[indexOfStartGrammar];
}

TokenDfa *PersistentData::getTokenDfaByInputStream() {
  int sizeOfTokenDfaStates = readInt();
  auto **tokenDfaStates = new TokenDfaState *[sizeOfTokenDfaStates];
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

  auto *tokenDfa = new TokenDfa(tokenDfaStates[0], (const TokenDfaState **) tokenDfaStates, sizeOfTokenDfaStates);
  return tokenDfa;
}

Grammar **PersistentData::getGrammarsByInputStream() {
  int tmpSizeOfGramamrs = readInt();
  auto **tmpGrammars = new Grammar *[tmpSizeOfGramamrs];

  for (int indexOfGrammars = 0; indexOfGrammars < tmpSizeOfGramamrs; indexOfGrammars++) {
    std::string *name = stringPool[readInt()];
    auto type = GrammarType(readInt());
    auto action = GrammarAction(readInt());
    auto *grammar = new Grammar(name, type, action);
    tmpGrammars[indexOfGrammars] = grammar;
  }
  this->sizeOfGramamrs = tmpSizeOfGramamrs;
  this->grammars = tmpGrammars;
  return tmpGrammars;
}

std::string **PersistentData::getStringPoolByInputStream() {
  int sizeOfString = readInt();
  auto **strings = new std::string *[sizeOfString];
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
  auto *str = new std::string(countOfStringBytes, 0);
  char *buf = const_cast<char *>(str->data());
  doRead((byte *) (buf), 0, countOfStringBytes);
  return str;
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
