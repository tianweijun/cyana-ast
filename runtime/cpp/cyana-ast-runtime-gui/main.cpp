#include "AstGuiOutputer.h"
#include "RuntimeAutomataAstApplication.h"
#include "ast.h"
#include <string>

int main(int argc, char* argv[]) {
  const std::string automataFilePath = "D:/java-ws/diy-test/src/automata.data";
  const std::string sourceCodeFilePath = "D:/java-ws/cyana-ast/src/cyana/ast/test/c/helloworld.c";

  RuntimeAutomataAstApplication runtimeAstApplication;
  runtimeAstApplication.setContext(&automataFilePath);

  const Ast *ast = runtimeAstApplication.buildAst(&sourceCodeFilePath);
  AstGuiOutputer astGuiOutputer(ast);
  astGuiOutputer.output();
  astGuiOutputer.waitToClose();
  delete ast;
  ast = 0;

  return 0;
}
