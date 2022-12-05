//
// Created by tian wei jun on 2022/11/24 0024.
//

#include "PersistentData.h"
#include "CyanaAstRuntimeException.h"
#include "SyntaxDfaState.h"

PersistentData::PersistentData(const std::string *automataFilePath) : inputStream(std::ifstream()),
                                                                      intByteBuffer(ByteBuffer(4, true)),
                                                                      stringPool(0),
                                                                      sizeOfStringPool(0), sizeOfGramamrs(0),
                                                                      productionRules(0), sizeOfProductionRules(0) {
  init(automataFilePath);
}

void PersistentData::init(const std::string *automataFilePath) {
  inputStream.open(*automataFilePath, std::ios::in | std::ios::binary);

  if (!inputStream.is_open()) {
    throw CyanaAstRuntimeException("open automata File error,path:'" + *automataFilePath + "'");
  }
}

PersistentData::~PersistentData() {
  inputStream.close();

  for (int i = 0; i < sizeOfProductionRules; i++) {
    ProductionRule *productionRule = productionRules[i];
    delete productionRule;
    productionRule = 0;
  }
  delete[] productionRules;
  productionRules = 0;

  for (int i = 0; i < sizeOfGramamrs; i++) {
    Grammar *grammar = grammars[i];
    delete grammar;
    grammar = 0;
  }
  delete[] grammars;
  grammars = 0;

  if (stringPool != 0) {
    for (int i = 0; i < sizeOfStringPool; i++) {
      std::string *string = stringPool[i];
      delete string;
      string = 0;
    }
    delete[] stringPool;
    stringPool = 0;
  }
}

void PersistentData::getProductionRulesByInputStream() {
  int countOfProductionRules = readInt();
  ProductionRule **productionRules = new ProductionRule *[countOfProductionRules];
  for (int indexOfProductionRules = 0;
       indexOfProductionRules < countOfProductionRules;
       indexOfProductionRules++) {
    productionRules[indexOfProductionRules] = new ProductionRule();
  }
  this->productionRules = productionRules;

  for (int indexOfProductionRules = 0;
       indexOfProductionRules < countOfProductionRules;
       indexOfProductionRules++) {
    ProductionRule *productionRule = productionRules[indexOfProductionRules];
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
  SyntaxDfaState **syntaxDfaStates = new SyntaxDfaState *[sizeOfSyntaxDfaStates];
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
  SyntaxDfa *syntaxDfa = new SyntaxDfa();
  syntaxDfa->states = syntaxDfaStates;
  syntaxDfa->start = syntaxDfaStates[0];
  return syntaxDfa;
}

Grammar *PersistentData::getStartGrammarByInputStream() {
  int indexOfStartGrammar = readInt();
  return grammars[indexOfStartGrammar];
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
  int sizeOfGramamrs = readInt();
  Grammar **grammars = new Grammar *[sizeOfGramamrs];

  for (int indexOfGrammars = 0; indexOfGrammars < sizeOfGramamrs; indexOfGrammars++) {
    std::string *name = stringPool[readInt()];
    Grammar *grammar = new Grammar(name);
    grammar->type = GrammarType(readInt());
    grammar->action = GrammarAction(readInt());
    grammars[indexOfGrammars] = grammar;
  }
  this->sizeOfGramamrs = sizeOfGramamrs;
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
