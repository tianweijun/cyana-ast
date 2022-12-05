package cyana.ast.runtime;

import java.io.InputStream;

/**
 * RuntimeAutomataAstApplication.
 *
 * @author tian wei jun
 */
public class RuntimeAutomataAstApplication {
  PersistentAutomataAstApplication persistentAutomataAstApplication;

  public Ast buildAst(String sourceFilePath) {
    return persistentAutomataAstApplication.buildAst(sourceFilePath);
  }

  public Ast buildAst(InputStream sourceInputStream) {
    Ast ast = persistentAutomataAstApplication.buildAst(sourceInputStream);
    try {
      if (null != sourceInputStream) {
        sourceInputStream.close();
      }
    } catch (Exception e) {
      throw new CyanaAstRuntimeException(e);
    }
    return ast;
  }

  public void setContext(InputStream automataInputStream) {
    // automataInputStream 会被下层模块关闭
    persistentAutomataAstApplication = new PersistentAutomataAstApplication(automataInputStream);
  }

  public void setContext(String automataFilePath) {
    persistentAutomataAstApplication = new PersistentAutomataAstApplication(automataFilePath);
  }

  public void displayGraphicalViewOfAst(Ast ast) {
    new cyana.ast.runtime.AstGuiOutputer().output(ast);
  }
}
