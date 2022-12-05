package cyana.ast.runtime;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;

/**
 * 抽象语法树.
 *
 * @author tian wei jun
 */
public class Ast {
  public Grammar grammar = null;
  public String alias = "";
  // grammar.type == GrammarType.TERMINAL
  public Token token = null;

  public Ast parent = null;
  public LinkedList<Ast> children = new LinkedList<>();

  public Ast() {}

  public Ast(ProductionRule productionRule) {
    grammar = productionRule.grammar;
    alias = productionRule.alias;
  }

  public Ast(Token token) {
    this.grammar = token.terminal;
    this.token = token;
  }

  /**
   * parent一定要为null.
   *
   * @return
   */
  public Ast clone() {
    if (null != parent) {
      throw new CyanaAstRuntimeException("parent of ast must be null in clone of Ast");
    }
    return new AstCloner(this).clone();
  }

  @Override
  public String toString() {
    String displayString = "";
    GrammarType type = grammar.type;
    if (type == GrammarType.TERMINAL) {
      displayString = token.text;
    }
    if (type == GrammarType.NONTERMINAL) {
      if (StringUtils.isNotBlank(alias)) {
        displayString = grammar.name + "[" + alias + "]";
      } else {
        displayString = grammar.name;
      }
    }
    return displayString;
  }

  public static class AstCloner {
    Ast source;
    Map<Ast, Ast> sourceDestAstMap;
    LinkedList<Ast> sourceAsts = new LinkedList<>();

    public AstCloner(Ast source) {
      this.source = source;
      setSourceAsts(source);
    }

    public Ast clone() {
      mapSourceDestAst();
      cloneAst();
      return sourceDestAstMap.get(source);
    }

    private void cloneAst() {
      for (Ast sourceAst : sourceAsts) {
        Ast destAst = sourceDestAstMap.get(sourceAst);
        destAst.grammar = sourceAst.grammar;
        destAst.alias = sourceAst.alias;
        destAst.token = sourceAst.token;
        if (null != sourceAst.parent) {
          destAst.parent = sourceDestAstMap.get(sourceAst.parent);
        }
        for (Ast childOfSourceAst : sourceAst.children) {
          destAst.children.add(sourceDestAstMap.get(childOfSourceAst));
        }
      }
    }

    private void mapSourceDestAst() {
      sourceDestAstMap = new HashMap<>(sourceAsts.size());
      for (Ast sourceAst : sourceAsts) {
        Ast ast = new Ast();
        sourceDestAstMap.put(sourceAst, ast);
      }
    }

    private void setSourceAsts(Ast ast) {
      sourceAsts.add(ast);
      for (Ast child : ast.children) {
        setSourceAsts(child);
      }
    }
  }
}
