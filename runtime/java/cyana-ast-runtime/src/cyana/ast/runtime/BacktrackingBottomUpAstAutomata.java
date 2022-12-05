package cyana.ast.runtime;

import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.ListIterator;

/**
 * 按照可回溯方式构造ast的自动机.
 *
 * @author tian wei jun
 */
public class BacktrackingBottomUpAstAutomata {
  SyntaxDfa astDfa = null;
  Grammar startGrammar = null;
  TokenReducingSymbolInputStream tokenReducingSymbolInputStream = null;
  LinkedList<BacktrackingBottomUpBranch> bottomUpBranchs = new LinkedList<>();
  HashSet<BacktrackingBottomUpBranch> triedBottomUpBranchs = new HashSet<>();
  Ast result = null;

  public BacktrackingBottomUpAstAutomata(SyntaxDfa astDfa, Grammar startGrammar) {
    this.astDfa = astDfa;
    this.startGrammar = startGrammar;
  }

  /**
   * 按照所有可能的产生式归约.
   *
   * @param sourceTokens token数据流
   * @return
   */
  public Ast buildAst(List<Token> sourceTokens) {
    init(sourceTokens);
    while (null == result && !bottomUpBranchs.isEmpty()) {
      consumeBottomUpBranch();
    }
    return result;
  }

  private void consumeBottomUpBranch() {
    BacktrackingBottomUpBranch bottomUpBranch = bottomUpBranchs.getFirst();
    switch (bottomUpBranch.status) {
      case CREATED:
        reduceBottomUpBranch(bottomUpBranch);
        break;
      case REDUCED:
        shiftBottomUpBranch(bottomUpBranch);
        break;
      case SHIFTED:
        closeBottomUpBranch(bottomUpBranch);
        break;
      case NON_ACCEPTED:
        bottomUpBranchs.removeFirst();
        break;
      case ACCEPTED:
        result = bottomUpBranch.reducingSymbols.getLast().astOfCurrentDfaState;
        bottomUpBranchs.removeFirst();
        break;
      default:
    }
  }

  private void shiftBottomUpBranch(BacktrackingBottomUpBranch bottomUpBranch) {
    ReducingSymbol topReducingSymbol = bottomUpBranch.reducingSymbols.getLast();
    // 将输入流定位到分支读取的位置
    tokenReducingSymbolInputStream.nextReadIndex = topReducingSymbol.endIndexOfToken + 1;
    // 移进一个token
    if (tokenReducingSymbolInputStream.hasNext()) {
      Token token = tokenReducingSymbolInputStream.read();
      SyntaxDfaState nextDfaState = topReducingSymbol.currentDfaState.edges.get(token.terminal);
      // 连通的
      if (null != nextDfaState) {
        BacktrackingBottomUpBranch terminalBottomUpBranch = bottomUpBranch.clone();
        terminalBottomUpBranch.status = BacktrackingBottomUpBranch.Status.CREATED;
        // 归约的符号
        ReducingSymbol terminalReducingSymbol = new ReducingSymbol();
        terminalReducingSymbol.reducedGrammar = token.terminal;
        terminalReducingSymbol.astOfCurrentDfaState = new Ast(token);
        terminalReducingSymbol.currentDfaState = nextDfaState;
        terminalReducingSymbol.endIndexOfToken = tokenReducingSymbolInputStream.nextReadIndex - 1;
        // 归约的符号进栈
        terminalBottomUpBranch.reducingSymbols.addLast(terminalReducingSymbol);
        addNewBacktrackingBottomUpBranch(terminalBottomUpBranch);
      }
    }
    bottomUpBranch.status = BacktrackingBottomUpBranch.Status.SHIFTED;
  }

  private void reduceBottomUpBranch(BacktrackingBottomUpBranch bottomUpBranch) {
    ReducingSymbol topReducingSymbol = bottomUpBranch.reducingSymbols.getLast();
    SyntaxDfaState currentDfaState = topReducingSymbol.currentDfaState;
    if (!currentDfaState.closingProductionRules.isEmpty()) {
      for (ProductionRule closingProductionRule : currentDfaState.closingProductionRules) {
        doReduce(bottomUpBranch, closingProductionRule);
      }
    }
    bottomUpBranch.status = BacktrackingBottomUpBranch.Status.REDUCED;
  }

  private void doReduce(
      BacktrackingBottomUpBranch bottomUpBranch, ProductionRule closingProductionRule) {
    int endIndexOfToken = bottomUpBranch.reducingSymbols.getLast().endIndexOfToken;
    // 空归约
    if (FaStateType.isClosingTag(closingProductionRule.reversedDfa.start.type)) {
      SyntaxDfaState topReducingSymbolDfaState =
          bottomUpBranch.reducingSymbols.getLast().currentDfaState;
      SyntaxDfaState nextDfaState =
          topReducingSymbolDfaState.edges.get(closingProductionRule.grammar);
      // 连通的
      if (null != nextDfaState) {
        BacktrackingBottomUpBranch newBottomUpBranch = bottomUpBranch.clone();
        newBottomUpBranch.status = BacktrackingBottomUpBranch.Status.CREATED;
        // 归约的符号
        ReducingSymbol terminalReducingSymbol = new ReducingSymbol();
        terminalReducingSymbol.reducedGrammar = closingProductionRule.grammar;
        terminalReducingSymbol.astOfCurrentDfaState = new Ast(closingProductionRule);
        terminalReducingSymbol.currentDfaState = nextDfaState;
        terminalReducingSymbol.endIndexOfToken = endIndexOfToken;
        // 归约的符号进栈
        newBottomUpBranch.reducingSymbols.addLast(terminalReducingSymbol);
        addNewBacktrackingBottomUpBranch(newBottomUpBranch);
      }
    }
    // 非空归约
    SyntaxDfaState productionRuleDfaState = closingProductionRule.reversedDfa.start;
    int countOfComsumedReducingSymbol = 0;
    ListIterator<ReducingSymbol> reducingSymbolListIt =
        bottomUpBranch.reducingSymbols.listIterator(bottomUpBranch.reducingSymbols.size());
    while (reducingSymbolListIt.hasPrevious()) {
      // 读取一个归约符号
      ReducingSymbol inputReducingSymbol = reducingSymbolListIt.previous();
      ++countOfComsumedReducingSymbol;
      if (!reducingSymbolListIt.hasPrevious()) { // 栈顶都没有，直接结束
        break;
      }
      SyntaxDfaState nextProductionRuleDfaState =
          productionRuleDfaState.edges.get(inputReducingSymbol.reducedGrammar);
      if (null == nextProductionRuleDfaState) { // 无法按照产生式向前归约，结束
        break;
      }
      if (FaStateType.isClosingTag(nextProductionRuleDfaState.type)) {
        SyntaxDfaState topReducingSymbolDfaState = reducingSymbolListIt.previous().currentDfaState;
        reducingSymbolListIt.next(); // 回到原来的位置，等价于peek了前一个元素
        SyntaxDfaState nextDfaState =
            topReducingSymbolDfaState.edges.get(closingProductionRule.grammar);
        // 连通的
        if (null != nextDfaState) {
          BacktrackingBottomUpBranch newBottomUpBranch = bottomUpBranch.clone();
          newBottomUpBranch.status = BacktrackingBottomUpBranch.Status.CREATED;
          // 被归约的符号出栈，同时建立语法树孩子节点
          Ast reducingAst = new Ast(closingProductionRule);
          for (int countOfUselessReducingSymbol = 1;
              countOfUselessReducingSymbol <= countOfComsumedReducingSymbol;
              countOfUselessReducingSymbol++) {
            ReducingSymbol childReducingSymbol = newBottomUpBranch.reducingSymbols.removeLast();
            // 不用克隆孩子语法树，因为newBottomUpBranch已经克隆了原来分支的符号栈信息，
            // 现在又把这些产生式孩子元素丢弃，故这些孩子语法树直接拿来用，使其不被回收
            childReducingSymbol.astOfCurrentDfaState.parent = reducingAst;
            reducingAst.children.addFirst(childReducingSymbol.astOfCurrentDfaState);
          }
          // 归约的符号
          ReducingSymbol terminalReducingSymbol = new ReducingSymbol();
          terminalReducingSymbol.reducedGrammar = closingProductionRule.grammar;
          terminalReducingSymbol.endIndexOfToken = endIndexOfToken;
          terminalReducingSymbol.currentDfaState = nextDfaState;
          terminalReducingSymbol.astOfCurrentDfaState = reducingAst;
          // 归约的符号进栈
          newBottomUpBranch.reducingSymbols.addLast(terminalReducingSymbol);
          addNewBacktrackingBottomUpBranch(newBottomUpBranch);
        }
      }
      productionRuleDfaState = nextProductionRuleDfaState;
    }
  }

  private void closeBottomUpBranch(BacktrackingBottomUpBranch bottomUpBranch) {
    ReducingSymbol topReducingSymbol = bottomUpBranch.reducingSymbols.getLast();
    tokenReducingSymbolInputStream.nextReadIndex = topReducingSymbol.endIndexOfToken + 1;
    bottomUpBranch.status = BacktrackingBottomUpBranch.Status.NON_ACCEPTED;
    // 可接受状态:栈中有两个归约，栈底是基准标志，栈顶是归约结果，并且源文件输入流全部识别了
    if (tokenReducingSymbolInputStream.hasReadAll()
        && startGrammar.equals(topReducingSymbol.reducedGrammar)) {
      bottomUpBranch.status = BacktrackingBottomUpBranch.Status.ACCEPTED;
    }
  }

  private void init(List<Token> sourceTokens) {
    tokenReducingSymbolInputStream = new TokenReducingSymbolInputStream(sourceTokens);
    bottomUpBranchs.clear();
    result = null;
    triedBottomUpBranchs.clear();
    ReducingSymbol connectedSignOfStartGrammarReducingSymbol =
        getConnectedSignOfStartGrammarReducingSymbol();
    SyntaxDfaState topReducingSymbolDfaState =
        connectedSignOfStartGrammarReducingSymbol.currentDfaState;
    // 空归约
    for (ProductionRule closingProductionRule : topReducingSymbolDfaState.closingProductionRules) {
      SyntaxDfaState nextDfaState =
          topReducingSymbolDfaState.edges.get(closingProductionRule.grammar);
      if (null == nextDfaState) {
        continue;
      }
      // 连通的
      BacktrackingBottomUpBranch nonterminalBottomUpBranch = new BacktrackingBottomUpBranch();
      // 基准标志进栈
      nonterminalBottomUpBranch.reducingSymbols.addLast(
          connectedSignOfStartGrammarReducingSymbol.clone());
      // 归约的符号
      ReducingSymbol nonterminalReducingSymbol = new ReducingSymbol();
      nonterminalReducingSymbol.reducedGrammar = closingProductionRule.grammar;
      nonterminalReducingSymbol.astOfCurrentDfaState = new Ast(closingProductionRule);
      nonterminalReducingSymbol.currentDfaState = nextDfaState;
      nonterminalReducingSymbol.endIndexOfToken = -1;
      // 归约的符号进栈
      nonterminalBottomUpBranch.reducingSymbols.addLast(nonterminalReducingSymbol);
      addNewBacktrackingBottomUpBranch(nonterminalBottomUpBranch);
    }
    // 移进一个token
    if (tokenReducingSymbolInputStream.hasNext()) {
      // 将输入流定位到分支读取的位置,因为是初始化，所以为0
      tokenReducingSymbolInputStream.nextReadIndex = 0;
      Token token = tokenReducingSymbolInputStream.read();
      SyntaxDfaState nextDfaState = topReducingSymbolDfaState.edges.get(token.terminal);
      // 连通的
      if (null != nextDfaState) {
        BacktrackingBottomUpBranch terminalBottomUpBranch = new BacktrackingBottomUpBranch();
        // 基准标志进栈
        terminalBottomUpBranch.reducingSymbols.addLast(
            connectedSignOfStartGrammarReducingSymbol.clone());
        // 归约的符号
        ReducingSymbol terminalReducingSymbol = new ReducingSymbol();
        terminalReducingSymbol.reducedGrammar = token.terminal;
        terminalReducingSymbol.astOfCurrentDfaState = new Ast(token);
        terminalReducingSymbol.currentDfaState = nextDfaState;
        terminalReducingSymbol.endIndexOfToken = tokenReducingSymbolInputStream.nextReadIndex - 1;
        // 归约的符号进栈
        terminalBottomUpBranch.reducingSymbols.addLast(terminalReducingSymbol);
        addNewBacktrackingBottomUpBranch(terminalBottomUpBranch);
      }
    }
  }

  /**
   * 增广文法，相当于一个连通标志，主要作用是开始文法归约连通判断.
   *
   * @return
   */
  private ReducingSymbol getConnectedSignOfStartGrammarReducingSymbol() {
    ReducingSymbol connectedSignOfStartGrammarReducingSymbol = new ReducingSymbol();
    connectedSignOfStartGrammarReducingSymbol.reducedGrammar = null;
    connectedSignOfStartGrammarReducingSymbol.astOfCurrentDfaState = null;
    connectedSignOfStartGrammarReducingSymbol.endIndexOfToken = -1;
    connectedSignOfStartGrammarReducingSymbol.currentDfaState = astDfa.start;
    return connectedSignOfStartGrammarReducingSymbol;
  }

  private void addNewBacktrackingBottomUpBranch(
      BacktrackingBottomUpBranch newBacktrackingBottomUpBranch) {
    if (triedBottomUpBranchs.contains(newBacktrackingBottomUpBranch)) {
      return;
    }
    bottomUpBranchs.add(newBacktrackingBottomUpBranch);
    triedBottomUpBranchs.add(newBacktrackingBottomUpBranch.uniqueIdentificationClone());
  }
}
