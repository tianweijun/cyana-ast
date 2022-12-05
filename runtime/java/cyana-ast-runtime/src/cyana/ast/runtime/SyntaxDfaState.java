package cyana.ast.runtime;

import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.Map;

/**
 * 语法对应的 确定有限状态自动机 的状态.
 *
 * @author tian wei jun
 */
public class SyntaxDfaState {
  public int type = FaStateType.NONE.getValue();
  // 转移
  public Map<Grammar, SyntaxDfaState> edges = new LinkedHashMap<>();
  public LinkedList<ProductionRule> closingProductionRules = new LinkedList<>();
}
