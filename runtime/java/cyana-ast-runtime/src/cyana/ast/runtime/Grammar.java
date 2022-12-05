package cyana.ast.runtime;

/**
 * 语法（文法）.
 *
 * @author tian wei jun
 */
public class Grammar {
  public String name = "";
  public GrammarType type;
  public GrammarAction action = GrammarAction.TEXT;
}
