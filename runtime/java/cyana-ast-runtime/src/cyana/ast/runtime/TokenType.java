package cyana.ast.runtime;

/**
 * 识别文本（通常为源代码）的 token的 类型.
 *
 * @author tian wei jun
 */
public enum TokenType {
  TEXT("text"),
  SKIP("skip");

  private String name;

  TokenType(String name) {
    this.name = name;
  }

  public String getName() {
    return name;
  }

  /**
   * 根据GrammarAction获取TokenType.
   *
   * @param grammarAction GrammarAction
   * @return 识别文本（通常为源代码）的 token的 类型
   */
  public static TokenType getByGrammarAction(GrammarAction grammarAction) {
    TokenType tokenType = TEXT;
    for (TokenType type : TokenType.values()) {
      if (type.getName().equals(grammarAction.getName())) {
        tokenType = type;
        break;
      }
    }
    return tokenType;
  }

  @Override
  public String toString() {
    return name;
  }
}
