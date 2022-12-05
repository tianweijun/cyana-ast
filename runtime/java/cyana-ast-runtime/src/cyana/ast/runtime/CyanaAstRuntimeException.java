package cyana.ast.runtime;

/**
 * 自定义运行时异常.
 *
 * @author tian wei jun
 */
public class CyanaAstRuntimeException extends RuntimeException {

  public CyanaAstRuntimeException(Throwable cause) {
    super(cause);
  }

  public CyanaAstRuntimeException(String message) {
    super(message);
  }
}
