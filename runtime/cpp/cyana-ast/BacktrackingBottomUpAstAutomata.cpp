//
// Created by tian wei jun on 2022/11/25 0025.
//

#include "BacktrackingBottomUpAstAutomata.h"
#include "FaStateType.h"

BacktrackingBottomUpAstAutomata::BacktrackingBottomUpAstAutomata(SyntaxDfa *astDfa, Grammar *startGrammar)
    : astDfa(astDfa), startGrammar(startGrammar), result(0),
      bottomUpBranchs(std::list<BacktrackingBottomUpBranch *>()),
      triedBottomUpBranchs(std::set<BacktrackingBottomUpBranch *, BacktrackingBottomUpBranchCompare>()),
      tokenReducingSymbolInputStream(0) {
}

BacktrackingBottomUpAstAutomata::~BacktrackingBottomUpAstAutomata() {
  //astDfa delete by PersistentObject.astDfa
  //startGrammar delete by persistentData.grammars
  //result delete by caller
  delete tokenReducingSymbolInputStream;
  tokenReducingSymbolInputStream = 0;

  for (auto bottomUpBranchsIt = bottomUpBranchs.begin(); bottomUpBranchsIt != bottomUpBranchs.end(); bottomUpBranchsIt++) {
    BacktrackingBottomUpBranch *backtrackingBottomUpBranch = *bottomUpBranchsIt;
    delete backtrackingBottomUpBranch;
    backtrackingBottomUpBranch = 0;
  }
  bottomUpBranchs.clear();

  for (auto bottomUpBranchsIt = triedBottomUpBranchs.begin(); bottomUpBranchsIt != triedBottomUpBranchs.end(); bottomUpBranchsIt++) {
    BacktrackingBottomUpBranch *backtrackingBottomUpBranch = *bottomUpBranchsIt;
    delete backtrackingBottomUpBranch;
    backtrackingBottomUpBranch = 0;
  }
  triedBottomUpBranchs.clear();
}

const Ast *BacktrackingBottomUpAstAutomata::buildAst(std::list<Token *> *sourceTokens) {
  init(sourceTokens);
  while (0 == result && !bottomUpBranchs.empty()) {
    consumeBottomUpBranch();
  }
  return result;
}

void BacktrackingBottomUpAstAutomata::consumeBottomUpBranch() {
  BacktrackingBottomUpBranch *bottomUpBranch = bottomUpBranchs.front();
  switch (bottomUpBranch->status) {
    case BacktrackingBottomUpBranchStatus::CREATED:
      reduceBottomUpBranch(bottomUpBranch);
      break;
    case BacktrackingBottomUpBranchStatus::REDUCED:
      shiftBottomUpBranch(bottomUpBranch);
      break;
    case BacktrackingBottomUpBranchStatus::SHIFTED:
      closeBottomUpBranch(bottomUpBranch);
      break;
    case BacktrackingBottomUpBranchStatus::NON_ACCEPTED:
      bottomUpBranchs.pop_front();
      delete bottomUpBranch;
      break;
    case BacktrackingBottomUpBranchStatus::ACCEPTED:
      result = bottomUpBranch->reducingSymbols.back()->astOfCurrentDfaState->clone();
      bottomUpBranchs.pop_front();
      delete bottomUpBranch;
      break;
    default:
      break;
  }
}

void BacktrackingBottomUpAstAutomata::reduceBottomUpBranch(BacktrackingBottomUpBranch *bottomUpBranch) {
  ReducingSymbol *topReducingSymbol = bottomUpBranch->reducingSymbols.back();
  const SyntaxDfaState *currentDfaState = topReducingSymbol->currentDfaState;
  if (!currentDfaState->closingProductionRules.empty()) {
    auto &closingProductionRules = currentDfaState->closingProductionRules;
    for (auto closingProductionRulesIt = closingProductionRules.begin(); closingProductionRulesIt != closingProductionRules.end(); closingProductionRulesIt++) {
      auto closingProductionRule = *closingProductionRulesIt;
      doReduce(bottomUpBranch, closingProductionRule);
    }
  }
  bottomUpBranch->status = BacktrackingBottomUpBranchStatus::REDUCED;
}

void BacktrackingBottomUpAstAutomata::doReduce(BacktrackingBottomUpBranch *bottomUpBranch, ProductionRule *closingProductionRule) {
  int endIndexOfToken = bottomUpBranch->reducingSymbols.back()->endIndexOfToken;
  // 空归约
  if (FaStateType::isClosingTag(closingProductionRule->reversedDfa->start->type)) {
    const SyntaxDfaState *topReducingSymbolDfaState =
        bottomUpBranch->reducingSymbols.back()->currentDfaState;
    SyntaxDfaState *nextDfaState = 0;
    auto nextDfaStateIt = topReducingSymbolDfaState->edges.find(closingProductionRule->grammar);
    if (nextDfaStateIt != topReducingSymbolDfaState->edges.end()) {
      nextDfaState = nextDfaStateIt->second;
    }
    // 连通的
    if (0 != nextDfaState) {
      BacktrackingBottomUpBranch *newBottomUpBranch = bottomUpBranch->clone();
      newBottomUpBranch->status = BacktrackingBottomUpBranchStatus::CREATED;
      // 归约的符号
      ReducingSymbol *terminalReducingSymbol = new ReducingSymbol();
      terminalReducingSymbol->reducedGrammar = closingProductionRule->grammar;
      terminalReducingSymbol->astOfCurrentDfaState = new Ast(closingProductionRule);
      terminalReducingSymbol->currentDfaState = nextDfaState;
      terminalReducingSymbol->endIndexOfToken = endIndexOfToken;
      // 归约的符号进栈
      newBottomUpBranch->reducingSymbols.push_back(terminalReducingSymbol);
      addNewBacktrackingBottomUpBranch(newBottomUpBranch);
    }
  }
  // 非空归约
  SyntaxDfaState *productionRuleDfaState = closingProductionRule->reversedDfa->start;
  int countOfComsumedReducingSymbol = 0;
  ListIterator<ReducingSymbol> reducingSymbolListIt =
      bottomUpBranch.reducingSymbols.listIterator(bottomUpBranch.reducingSymbols.size());
  while (reducingSymbolListIt.hasPrevious()) {
    // 读取一个归约符号
    ReducingSymbol inputReducingSymbol = reducingSymbolListIt.previous();
    ++countOfComsumedReducingSymbol;
    if (!reducingSymbolListIt.hasPrevious()) {// 栈顶都没有，直接结束
      break;
    }
    SyntaxDfaState nextProductionRuleDfaState =
        productionRuleDfaState.edges.get(inputReducingSymbol.reducedGrammar);
    if (null == nextProductionRuleDfaState) {// 无法按照产生式向前归约，结束
      break;
    }
    if (FaStateType.isClosingTag(nextProductionRuleDfaState.type)) {
      SyntaxDfaState topReducingSymbolDfaState = reducingSymbolListIt.previous().currentDfaState;
      reducingSymbolListIt.next();// 回到原来的位置，等价于peek了前一个元素
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

void BacktrackingBottomUpAstAutomata::shiftBottomUpBranch(BacktrackingBottomUpBranch *bottomUpBranch) {
  ReducingSymbol *topReducingSymbol = bottomUpBranch->reducingSymbols.back();
  // 将输入流定位到分支读取的位置
  tokenReducingSymbolInputStream->nextReadIndex = topReducingSymbol->endIndexOfToken + 1;
  // 移进一个token
  if (tokenReducingSymbolInputStream->hasNext()) {
    Token *token = tokenReducingSymbolInputStream->read();
    SyntaxDfaState *nextDfaState = 0;
    auto nextDfaStateIt = topReducingSymbol->currentDfaState->edges.find(token->terminal);
    if (nextDfaStateIt != topReducingSymbol->currentDfaState->edges.end()) {
      nextDfaState = nextDfaStateIt->second;
    }
    // 连通的
    if (0 != nextDfaState) {
      BacktrackingBottomUpBranch *terminalBottomUpBranch = bottomUpBranch->clone();
      terminalBottomUpBranch->status = BacktrackingBottomUpBranchStatus::CREATED;
      // 归约的符号
      ReducingSymbol *terminalReducingSymbol = new ReducingSymbol();
      terminalReducingSymbol->reducedGrammar = token->terminal;
      terminalReducingSymbol->astOfCurrentDfaState = new Ast(token);
      terminalReducingSymbol->currentDfaState = nextDfaState;
      terminalReducingSymbol->endIndexOfToken = tokenReducingSymbolInputStream->nextReadIndex - 1;
      // 归约的符号进栈
      terminalBottomUpBranch->reducingSymbols.push_back(terminalReducingSymbol);
      addNewBacktrackingBottomUpBranch(terminalBottomUpBranch);
    }
  }
  bottomUpBranch->status = BacktrackingBottomUpBranchStatus::SHIFTED;
}

void BacktrackingBottomUpAstAutomata::closeBottomUpBranch(BacktrackingBottomUpBranch *bottomUpBranch) {
  ReducingSymbol *topReducingSymbol = bottomUpBranch->reducingSymbols.back();
  tokenReducingSymbolInputStream->nextReadIndex = topReducingSymbol->endIndexOfToken + 1;
  bottomUpBranch->status = BacktrackingBottomUpBranchStatus::NON_ACCEPTED;
  // 可接受状态:栈中有两个归约，栈底是基准标志，栈顶是归约结果，并且源文件输入流全部识别了
  if (tokenReducingSymbolInputStream->hasReadAll()
      && startGrammar == topReducingSymbol->reducedGrammar) {
    bottomUpBranch->status = BacktrackingBottomUpBranchStatus::ACCEPTED;
  }
}

void BacktrackingBottomUpAstAutomata::init(std::list<Token *> *sourceTokens) {
  delete tokenReducingSymbolInputStream;
  tokenReducingSymbolInputStream = new TokenReducingSymbolInputStream(sourceTokens);
  result = 0;
  for (auto bottomUpBranchsIt = bottomUpBranchs.begin(); bottomUpBranchsIt != bottomUpBranchs.end(); bottomUpBranchsIt++) {
    BacktrackingBottomUpBranch *backtrackingBottomUpBranch = *bottomUpBranchsIt;
    delete backtrackingBottomUpBranch;
    backtrackingBottomUpBranch = 0;
  }
  bottomUpBranchs.clear();
  for (auto bottomUpBranchsIt = triedBottomUpBranchs.begin(); bottomUpBranchsIt != triedBottomUpBranchs.end(); bottomUpBranchsIt++) {
    BacktrackingBottomUpBranch *backtrackingBottomUpBranch = *bottomUpBranchsIt;
    delete backtrackingBottomUpBranch;
    backtrackingBottomUpBranch = 0;
  }
  triedBottomUpBranchs.clear();

  ReducingSymbol *connectedSignOfStartGrammarReducingSymbol =
      getConnectedSignOfStartGrammarReducingSymbol();
  const SyntaxDfaState *topReducingSymbolDfaState =
      connectedSignOfStartGrammarReducingSymbol->currentDfaState;
  // 空归约
  auto &closingProductionRules = topReducingSymbolDfaState->closingProductionRules;
  for (auto closingProductionRulesIt = closingProductionRules.begin(); closingProductionRulesIt != closingProductionRules.end(); closingProductionRulesIt++) {
    ProductionRule *closingProductionRule = *closingProductionRulesIt;
    SyntaxDfaState *nextDfaState = 0;
    auto nextDfaStateIt = topReducingSymbolDfaState->edges.find(closingProductionRule->grammar);
    if (nextDfaStateIt != topReducingSymbolDfaState->edges.end()) {
      nextDfaState = nextDfaStateIt->second;
    }
    if (0 == nextDfaState) {
      continue;
    }
    // 连通的
    BacktrackingBottomUpBranch *nonterminalBottomUpBranch = new BacktrackingBottomUpBranch();
    // 基准标志进栈
    nonterminalBottomUpBranch->reducingSymbols.push_back(
        connectedSignOfStartGrammarReducingSymbol->clone());
    // 归约的符号
    ReducingSymbol *nonterminalReducingSymbol = new ReducingSymbol();
    nonterminalReducingSymbol->reducedGrammar = closingProductionRule->grammar;
    nonterminalReducingSymbol->astOfCurrentDfaState = new Ast(closingProductionRule);
    nonterminalReducingSymbol->currentDfaState = nextDfaState;
    nonterminalReducingSymbol->endIndexOfToken = -1;
    // 归约的符号进栈
    nonterminalBottomUpBranch->reducingSymbols.push_back(nonterminalReducingSymbol);
    addNewBacktrackingBottomUpBranch(nonterminalBottomUpBranch);
  }
  // 移进一个token
  if (tokenReducingSymbolInputStream->hasNext()) {
    // 将输入流定位到分支读取的位置,因为是初始化，所以为0
    tokenReducingSymbolInputStream->nextReadIndex = 0;
    Token *token = tokenReducingSymbolInputStream->read();
    SyntaxDfaState *nextDfaState = 0;
    auto nextDfaStateIt = topReducingSymbolDfaState->edges.find(token->terminal);
    if (nextDfaStateIt != topReducingSymbolDfaState->edges.end()) {
      nextDfaState = nextDfaStateIt->second;
    }
    // 连通的
    if (0 != nextDfaState) {
      BacktrackingBottomUpBranch *terminalBottomUpBranch = new BacktrackingBottomUpBranch();
      // 基准标志进栈
      terminalBottomUpBranch->reducingSymbols.push_back(
          connectedSignOfStartGrammarReducingSymbol->clone());
      // 归约的符号
      ReducingSymbol *terminalReducingSymbol = new ReducingSymbol();
      terminalReducingSymbol->reducedGrammar = token->terminal;
      terminalReducingSymbol->astOfCurrentDfaState = new Ast(token);
      terminalReducingSymbol->currentDfaState = nextDfaState;
      terminalReducingSymbol->endIndexOfToken = tokenReducingSymbolInputStream->nextReadIndex - 1;
      // 归约的符号进栈
      terminalBottomUpBranch->reducingSymbols.push_back(terminalReducingSymbol);
      addNewBacktrackingBottomUpBranch(terminalBottomUpBranch);
    }
  }
}

ReducingSymbol *BacktrackingBottomUpAstAutomata::getConnectedSignOfStartGrammarReducingSymbol() {
  ReducingSymbol *connectedSignOfStartGrammarReducingSymbol = new ReducingSymbol();
  connectedSignOfStartGrammarReducingSymbol->reducedGrammar = 0;
  connectedSignOfStartGrammarReducingSymbol->astOfCurrentDfaState = 0;
  connectedSignOfStartGrammarReducingSymbol->endIndexOfToken = -1;
  connectedSignOfStartGrammarReducingSymbol->currentDfaState = astDfa->start;
  return connectedSignOfStartGrammarReducingSymbol;
}

void BacktrackingBottomUpAstAutomata::addNewBacktrackingBottomUpBranch(BacktrackingBottomUpBranch *newBacktrackingBottomUpBranch) {
  auto findNewBacktrackingBottomUpBranchIt = triedBottomUpBranchs.find(newBacktrackingBottomUpBranch);
  if (findNewBacktrackingBottomUpBranchIt != triedBottomUpBranchs.end()) {
    return;
  }
  bottomUpBranchs.push_back(newBacktrackingBottomUpBranch);
  triedBottomUpBranchs.insert(newBacktrackingBottomUpBranch->uniqueIdentificationClone());
}