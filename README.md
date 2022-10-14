# cyana-ast
generate Abstract Syntax Tree by grammar file.


# quick start
java -jar  cyana-ast.jar  -grammarFilePath D:/java-ws/cyana-ast/src/cyana/ast/test/c/C.grammar -sourceFilePath D:/java-ws/cyana-ast/src/cyana/ast/test/c/helloworld.c --graphicalViewOfAst

java -jar  cyana-ast.jar  -grammarFilePath  D:/java-ws/cyana-ast/src/cyana/ast/test/c/C.grammar -languageOfPersistentAutomata java -persistentAutomataFileDirectory D:/java-ws/diy-test/src

java -jar  cyana-ast.jar  -automataFilePath  D:/java-ws/diy-test/src/automata.data -sourceFilePath D:/java-ws/cyana-ast/src/cyana/ast/test/c/helloworld.c --graphicalViewOfAst

import cyana-ast.jar and using it like this:

```java
  public static void main(String[] args) {
    String automataFilePath = "D:/java-ws/diy-test/src/automata.data";
    String sourceCodeFilePath = "D:/java-ws/cyana-ast/src/cyana/ast/test/c/helloworld.c";

    CyanaAstApplication cyanaAstApplication = new CyanaAstApplication();
    cyanaAstApplication.setContextByAutomataFile(automataFilePath);

    Ast ast = cyanaAstApplication.buildAstByAutomataFile(sourceCodeFilePath);

    AstGuiOutputer astOutputer = new AstGuiOutputer();
    astOutputer.output(ast);
  }
```


# references

- Alfred V.Aho,Ravi Sethi,Jeffrey D.Ullman.*Compilers: Principles,Techniques,and Tools*.
