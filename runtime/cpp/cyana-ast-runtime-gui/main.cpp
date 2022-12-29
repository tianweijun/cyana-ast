#include "AstGuiOutputer.h"
#include <string>

int main(int argc, char *argv[]) {

  const std::string automataFilePath = "D:/java-ws/diy-test/src/automata.data";
  const std::string sourceCodeFilePath = "D:/java-ws/cyana-ast/src/cyana/ast/test/c/helloworld.c";

  RuntimeAutomataAstApplication runtimeAstApplication;
  runtimeAstApplication.setContext(&automataFilePath);
  //初始化错误（可能原因：自动机文件不存在）
  if (CyanaAstRuntimeExceptionResolver::hasThrewException()) {
    CyanaAstRuntimeExceptionResolver::clearExceptions();
    return 0;
  }
  const Ast *ast = runtimeAstApplication.buildAst(&sourceCodeFilePath);
  if (ast) {
    AstGuiOutputer astGuiOutputer(ast);
    astGuiOutputer.output();
    astGuiOutputer.waitToClose();
    delete ast;
  } else {
    CyanaAstRuntimeExceptionResolver::clearExceptions();
  }
  //app exit
  CyanaAstRuntimeExceptionResolver::destroy();
  return 0;
}
