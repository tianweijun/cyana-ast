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

int main(int argc, char *argv[]) {
  std::string automataFilePath = "D:/java-ws/diy-test/src/automata.data";
  std::string sourceCodeFilePath = "D:/java-ws/cyana-ast/src/cyana/ast/test/c/helloworld.c";

  RuntimeAutomataAstApplication runtimeAstApplication;
  runtimeAstApplication.setContext(automataFilePath);

  Ast *ast = runtimeAstApplication.buildAst(sourceCodeFilePath);
  AstGuiOutputer astGuiOutputer;
  astGuiOutputer.output(ast);

  std::cout << "Hello, World!" << std::endl;

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
