package cyana.ast.runtime;

import java.util.LinkedList;
import java.util.Objects;

/**
 * 自顶向上归约分支.
 *
 * @author tian wei jun
 */
public class BacktrackingBottomUpBranch {
  public Status status = Status.CREATED;
  LinkedList<ReducingSymbol> reducingSymbols = new LinkedList<>();

  public BacktrackingBottomUpBranch clone() {
    BacktrackingBottomUpBranch bottomUpBranch = new BacktrackingBottomUpBranch();
    bottomUpBranch.status = this.status;
    for (ReducingSymbol reducingSymbol : this.reducingSymbols) {
      bottomUpBranch.reducingSymbols.addLast(reducingSymbol.clone());
    }
    return bottomUpBranch;
  }

  // for BacktrackingBottomUpAstAutomata.triedBottomUpBranchs(set)
  public BacktrackingBottomUpBranch uniqueIdentificationClone() {
    BacktrackingBottomUpBranch bottomUpBranch = new BacktrackingBottomUpBranch();
    bottomUpBranch.status = this.status;
    for (ReducingSymbol reducingSymbol : this.reducingSymbols) {
      bottomUpBranch.reducingSymbols.addLast(reducingSymbol.uniqueIdentificationClone());
    }
    return bottomUpBranch;
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
    BacktrackingBottomUpBranch that = (BacktrackingBottomUpBranch) o;
    return status == that.status && reducingSymbols.equals(that.reducingSymbols);
  }

  @Override
  public int hashCode() {
    return Objects.hash(status, reducingSymbols);
  }

  @Override
  public String toString() {
    StringBuilder stringBuilder = new StringBuilder();
    stringBuilder.append("[").append(status.name()).append("]");
    for (ReducingSymbol reducingSymbol : reducingSymbols) {
      if (null != reducingSymbol.reducedGrammar) {
        stringBuilder.append("-").append(reducingSymbol.reducedGrammar.name);
      }
    }
    return stringBuilder.toString();
  }

  /** 自顶向上归约分支生命周期. */
  public static enum Status {
    CREATED,
    REDUCED,
    SHIFTED,
    NON_ACCEPTED,
    ACCEPTED;
  }
}
