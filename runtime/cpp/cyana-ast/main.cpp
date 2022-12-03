#include "AstGuiOutputer.h"
#include "CyanaAstRuntimeException.h"
#include "Logger.h"
#include "RuntimeAutomataAstApplication.h"
#include "ast.h"
#include "lib/CyanaAstGuiLib.h"
#include <future>
#include <iostream>
#include <string>

using namespace logger;

void testGuiLib();
StringTree *getStringTree();

int main(int argc, char *argv[]) {
  const std::string automataFilePath = "D:/java-ws/diy-test/src/automata.data";
  const std::string sourceCodeFilePath = "D:/java-ws/cyana-ast/src/cyana/ast/test/c/helloworld.c";

  RuntimeAutomataAstApplication runtimeAstApplication;
  runtimeAstApplication.setContext(automataFilePath);

  // testGuiLib();

  Ast *ast = runtimeAstApplication.buildAst(sourceCodeFilePath);
  AstGuiOutputer astGuiOutputer(ast);
  astGuiOutputer.output();
  astGuiOutputer.waitToClose();
  delete ast;
  ast = 0;

  char *testStr = new char[10];
  for (int i = 0; i < 10; i++) {
    testStr[i] = 'a' + i;
  }
  std::cout << testStr << std::endl;
  testStr[6] = 0;
  std::cout << testStr << std::endl;

  std::string testString(testStr, 10);
  delete testStr;
  testStr = 0;
  std::cout << testString << std::endl;

  // testGuiLib();

  // throw CyanaAstRuntimeException("something error");

  return 0;
}

StringTree *getStringTree() {
  StringTree *root = new StringTree();
  root->text = new std::string("root");

  StringTree *child11 = new StringTree(root);
  child11->text = new std::string("child11");

  StringTree *child12 = new StringTree(root);
  child12->text = new std::string("child12");

  StringTree *child13 = new StringTree(root);
  child13->text = new std::string("child13");

  root->children->push_back(child11);
  root->children->push_back(child12);
  root->children->push_back(child13);

  StringTree *child11_1 = new StringTree(child11);
  child11_1->text = new std::string("child11_1");
  child11->children->push_back(child11_1);

  return root;
}

void testGuiLib() {
  StringTree *stringTree = getStringTree();

  std::future<int> futureOfView =
      std::async(std::launch::async, CyanaAstGuiLib::showViewUntilClose, stringTree);
  futureOfView.get();

  delete stringTree;
  stringTree = 0;
}
