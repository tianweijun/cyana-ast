//
// Created by tian wei jun on 2022/11/23 0023.
//

#include "PersistentAutomataAstApplication.h"
#include "CyanaAstRuntimeException.h"
#include <iostream>
#include <list>

PersistentAutomataAstApplication::PersistentAutomataAstApplication() : persistentObject(nullptr),
                                                                       dfaTokenAutomata(nullptr),
                                                                       astAutomata(nullptr) {
}

PersistentAutomataAstApplication::PersistentAutomataAstApplication(const std::string *persistentDataFilePath) : PersistentAutomataAstApplication() {
  buildContext(persistentDataFilePath);
}
PersistentAutomataAstApplication::~PersistentAutomataAstApplication() {
  delete dfaTokenAutomata;
  dfaTokenAutomata = nullptr;
  delete astAutomata;
  astAutomata = nullptr;
  delete persistentObject;
  persistentObject = nullptr;
}

void PersistentAutomataAstApplication::buildContext(const std::string *persistentDataFilePath) {
  auto *persistentData = new PersistentData(persistentDataFilePath);
  //初始化错误（可能原因：自动机文件不存在）
  if (HandlerExceptionResolver::hasThrewException()) {
    return;
  }
  persistentObject = new PersistentObject(persistentData);
  dfaTokenAutomata = new DfaTokenAutomata(persistentObject->tokenDfa);
  astAutomata =
      new BacktrackingBottomUpAstAutomata(persistentObject->astDfa, persistentObject->startGrammar);
}

const Ast *PersistentAutomataAstApplication::buildAst(const std::string *sourceCodeFilePath) const {
  HandlerExceptionResolver::clearExceptions();
  std::list<Token *> *tokens = dfaTokenAutomata->buildToken(sourceCodeFilePath);
  // byteBufferedInputStream初始化错误（可能原因：源文件不存在）
  //text is not a token
  if (HandlerExceptionResolver::hasThrewException()) {
    if (tokens) {
      for (auto &token : *tokens) {
        delete token;
      }
      delete tokens;
    }
    return nullptr;
  }
  const Ast *ast = astAutomata->buildAst(tokens);
  for (auto token : *tokens) {
    delete token;
  }
  delete tokens;
  //clone ast error
  if (HandlerExceptionResolver::hasThrewException()) {
    delete ast;
    ast = nullptr;
  }
  return ast;
}
