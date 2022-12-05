package cyana.ast.runtime;

import java.util.Objects;

/**
 * 分析栈存放的文法符号.
 *
 * @author tian wei jun
 */
public class ReducingSymbol {
  // grammar
  public Grammar reducedGrammar = null;
  // ast
  public Ast astOfCurrentDfaState = null;
  // 状态
  public SyntaxDfaState currentDfaState = null;
  // token流中的位置
  public int endIndexOfToken = -1;

  public ReducingSymbol() {}

  public ReducingSymbol clone() {
    ReducingSymbol reducingSymbol = new ReducingSymbol();
    reducingSymbol.endIndexOfToken = this.endIndexOfToken;
    reducingSymbol.reducedGrammar = this.reducedGrammar;
    reducingSymbol.currentDfaState = this.currentDfaState;
    if (null != this.astOfCurrentDfaState) { // 归约分支栈底的ast可能为null
      reducingSymbol.astOfCurrentDfaState = this.astOfCurrentDfaState.clone();
    }
    return reducingSymbol;
  }

  // for BacktrackingBottomUpAstAutomata.triedBottomUpBranchs(set)
  public ReducingSymbol uniqueIdentificationClone() {
    ReducingSymbol reducingSymbol = new ReducingSymbol();
    reducingSymbol.endIndexOfToken = this.endIndexOfToken;
    reducingSymbol.reducedGrammar = this.reducedGrammar;
    reducingSymbol.currentDfaState = this.currentDfaState;
    return reducingSymbol;
  }

  // for BacktrackingBottomUpAstAutomata.triedBottomUpBranchs(set)
  @Override
  public boolean equals(Object o) {
    if (this == o) {
      return true;
    }
    if (o == null || getClass() != o.getClass()) {
      return false;
    }
    ReducingSymbol that = (ReducingSymbol) o;
    return endIndexOfToken == that.endIndexOfToken
        && Objects.equals(reducedGrammar, that.reducedGrammar)
        && currentDfaState.equals(that.currentDfaState);
  }

  @Override
  public int hashCode() {
    return Objects.hash(reducedGrammar, currentDfaState, endIndexOfToken);
  }
}
