package cyana.ast.runtime;

import java.io.InputStream;
import java.util.List;

/**
 * 生成语法树的应用.
 *
 * @author tian wei jun
 */
public class PersistentAutomataAstApplication {
  PersistentObject persistentObject;
  DfaTokenAutomata dfaTokenAutomata;
  BacktrackingBottomUpAstAutomata astAutomata;

  public PersistentAutomataAstApplication(String persistentDataFilePath) {
    PersistentData persistentData = new PersistentData(persistentDataFilePath);
    this.buildContext(persistentData);
  }

  public PersistentAutomataAstApplication(InputStream inputStream) {
    PersistentData persistentData = new PersistentData(inputStream);
    this.buildContext(persistentData);
  }

  public void buildContext(PersistentData persistentData) {
    persistentObject = new PersistentObject(persistentData);
    dfaTokenAutomata = new DfaTokenAutomata(persistentObject.tokenDfa);
    astAutomata =
        new BacktrackingBottomUpAstAutomata(persistentObject.astDfa, persistentObject.startGrammar);
  }

  public Ast buildAst(String sourceCodeFilePath) {
    List<Token> tokens = dfaTokenAutomata.buildToken(sourceCodeFilePath);
    Ast ast = astAutomata.buildAst(tokens);
    Logger.info("CyanaAstApplication buildAst", "build successfully");
    return ast;
  }

  public Ast buildAst(InputStream inputStream) {
    List<Token> tokens = dfaTokenAutomata.buildToken(inputStream);
    Ast ast = astAutomata.buildAst(tokens);
    Logger.info("CyanaAstApplication buildAst", "build successfully");
    return ast;
  }
}
