# cyana-ast
generate Abstract Syntax Tree by grammar file.


# quick start
java -jar  cyana-ast.jar  -grammarFilePath D:/java-ws/cyana-ast/src/cyana/ast/test/c/C.grammar -sourceFilePath D:/java-ws/cyana-ast/src/cyana/ast/test/c/helloworld.c --graphicalViewOfAst

java -jar  cyana-ast.jar  -grammarFilePath  D:/java-ws/cyana-ast/src/cyana/ast/test/c/C.grammar -languageOfPersistentAutomata java -persistentAutomataFileDirectory D:/java-ws/diy-test/src

java -jar  cyana-ast.jar  -automataFilePath  D:/java-ws/diy-test/src/automata.data -sourceFilePath D:/java-ws/cyana-ast/src/cyana/ast/test/c/helloworld.c --graphicalViewOfAst

import cyana-ast.jar and using it like this:

```java
    String automataFilePath = "D:/java-ws/diy-test/src/automata.data";
    String sourceCodeFilePath = "D:/java-ws/cyana-ast/src/cyana/ast/test/c/helloworld.c";

    RuntimeAutomataAstApplication runtimeAstApplication = new RuntimeAutomataAstApplication();
    runtimeAstApplication.setContext(automataFilePath);

    Ast ast = runtimeAstApplication.buildAst(sourceCodeFilePath);
    runtimeAstApplication.displayGraphicalViewOfAst(ast);
```


# references

- Alfred V.Aho,Ravi Sethi,Jeffrey D.Ullman.*Compilers: Principles,Techniques,and Tools*.
