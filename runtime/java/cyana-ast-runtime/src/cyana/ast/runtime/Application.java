package cyana.ast.runtime;

/**
 * Application for Test.
 *
 * @author tian wei jun
 */
public class Application {

  public static void main(String[] args) {
    String automataFilePath = "D:/java-ws/diy-test/src/automata.data";
    String sourceCodeFilePath = "D:/java-ws/cyana-ast/src/cyana/ast/test/c/helloworld.c";
    if(null!=args && args.length>=2){
      automataFilePath=args[0];
      sourceCodeFilePath=args[1];
    }
    RuntimeAutomataAstApplication runtimeAstApplication = new RuntimeAutomataAstApplication();
    runtimeAstApplication.setContext(automataFilePath);

    Ast ast = runtimeAstApplication.buildAst(sourceCodeFilePath);
    runtimeAstApplication.displayGraphicalViewOfAst(ast);
  }

}
