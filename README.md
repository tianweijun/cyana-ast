# cyana-ast
generate Abstract Syntax Tree by grammar file.


# quick start
A regular expression describes the format of grammars:

keywords of statement :  -> ; : # nfa(s,t) regexp

keywords of regular expression:  ()[]''?*+{}\s

default terminal grammar:  Epsilon

default nonterminal grammar:  augmentedNonterminal

format of grammar's definition : [nfa(start,end) regexp] nameOfGrammar : regularExpression [-> skip] ;

The grammar file should be encoded in 8bits(iso-8859-1) format.

each token separated by white spaces.tokens include  //, /*, */ and regExpUnit.

content of grammar file like this；

```html
@startGrammar compilationUnit ;

@nonterminalGrammar begin ;
argumentExpressionList
    :   assignmentExpression
    |   argumentExpressionList ',' assignmentExpression
    ;
...
@nonterminalGrammar end ;

@terminalGrammar begin ;
Int : 'int' ;
...
@terminalGrammar end ;

@terminalFragmentGrammar begin ;
NewlineFragment
	: 	'\r'? '\n'
	;
...
@terminalFragmentGrammar end ;
```

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



# Reporting Vulnerabilities

cyana takes security issues very seriously. If you have any concerns about cyana-ast or believe you have uncovered a vulnerability, please get in touch via the QQ Group Chatting room 214515321 or the e-mail address 1932252321@qq.com. In the message, try to provide a description of the issue and ideally a way of reproducing it. i will get back to you as soon as possible.




# references

- Alfred V.Aho,Ravi Sethi,Jeffrey D.Ullman.*Compilers: Principles,Techniques,and Tools*.