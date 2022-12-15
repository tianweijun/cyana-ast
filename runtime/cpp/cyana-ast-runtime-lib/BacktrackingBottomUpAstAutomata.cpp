//
// Created by tian wei jun on 2022/11/25 0025.
//

#include "BacktrackingBottomUpAstAutomata.h"
#include "AutomataTmpAst.h"
#include "FaStateType.h"
#include <iterator>

bool BacktrackingBottomUpBranchCompare::operator()(const BacktrackingBottomUpBranch *t1, const BacktrackingBottomUpBranch *t2) {
  return t1->compare(t2);
}

BacktrackingBottomUpAstAutomata::BacktrackingBottomUpAstAutomata(const SyntaxDfa *astDfa, const Grammar *startGrammar)
    : astDfa(astDfa), startGrammar(startGrammar), result(nullptr),
      bottomUpBranchs(std::list<BacktrackingBottomUpBranch *>()),
      triedBottomUpBranchs(std::set<BacktrackingBottomUpBranch *, BacktrackingBottomUpBranchCompare>()) {
}

BacktrackingBottomUpAstAutomata::~BacktrackingBottomUpAstAutomata() {
  //astDfa delete by PersistentObject.astDfa
  //startGrammar delete by persistentData.grammars
  //result delete by caller
  clear();
}
void BacktrackingBottomUpAstAutomata::clear() {
  tokenReducingSymbolInputStream.clear();

  for (auto backtrackingBottomUpBranch : bottomUpBranchs) {
    delete backtrackingBottomUpBranch;
    backtrackingBottomUpBranch = nullptr;
  }
  bottomUpBranchs.clear();

  for (auto backtrackingBottomUpBranch : triedBottomUpBranchs) {
    delete backtrackingBottomUpBranch;
    backtrackingBottomUpBranch = nullptr;
  }
  triedBottomUpBranchs.clear();
}

const Ast *BacktrackingBottomUpAstAutomata::buildAst(std::list<Token *> *sourceTokens) {
  init(sourceTokens);
  while (!result && !bottomUpBranchs.empty()) {
    consumeBottomUpBranch();
  }
  clear();
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
      const AutomataTmpAst *automataTmpAst = bottomUpBranch->reducingSymbols.back()->astOfCurrentDfaState;
      AutomataTmpAst2AstConverter automataTmpAst2AstConverter(automataTmpAst);
      result = automataTmpAst2AstConverter.convert();
      bottomUpBranchs.pop_front();
      delete bottomUpBranch;
      break;
  }
}

void BacktrackingBottomUpAstAutomata::reduceBottomUpBranch(BacktrackingBottomUpBranch *bottomUpBranch) {
  ReducingSymbol *topReducingSymbol = bottomUpBranch->reducingSymbols.back();
  const SyntaxDfaState *currentDfaState = topReducingSymbol->currentDfaState;
  if (!currentDfaState->closingProductionRules.empty()) {
    auto &closingProductionRules = currentDfaState->closingProductionRules;
    for (auto closingProductionRule : closingProductionRules) {
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
    SyntaxDfaState *nextDfaState = nullptr;
    auto nextDfaStateIt = topReducingSymbolDfaState->edges.find(closingProductionRule->grammar);
    if (nextDfaStateIt != topReducingSymbolDfaState->edges.end()) {
      nextDfaState = nextDfaStateIt->second;
    }
    // 连通的
    if (nextDfaState) {
      BacktrackingBottomUpBranch *newBottomUpBranch = bottomUpBranch->clone();
      newBottomUpBranch->status = BacktrackingBottomUpBranchStatus::CREATED;
      // 归约的符号
      auto *terminalReducingSymbol = new ReducingSymbol();
      terminalReducingSymbol->reducedGrammar = closingProductionRule->grammar;
      terminalReducingSymbol->astOfCurrentDfaState = new AutomataTmpAst(
          closingProductionRule->grammar, closingProductionRule->alias);
      terminalReducingSymbol->currentDfaState = nextDfaState;
      terminalReducingSymbol->endIndexOfToken = endIndexOfToken;
      // 归约的符号进栈
      newBottomUpBranch->reducingSymbols.push_back(terminalReducingSymbol);
      addNewBacktrackingBottomUpBranch(newBottomUpBranch);
    }
  }
  // 非空归约
  const SyntaxDfaState *productionRuleDfaState = closingProductionRule->reversedDfa->start;
  int countOfComsumedReducingSymbol = 0;
  auto reducingSymbolListIt = bottomUpBranch->reducingSymbols.rbegin();
  while (reducingSymbolListIt != bottomUpBranch->reducingSymbols.rend()) {
    // 读取一个归约符号
    ReducingSymbol *inputReducingSymbol = *reducingSymbolListIt;
    ++countOfComsumedReducingSymbol;
    if (countOfComsumedReducingSymbol >= bottomUpBranch->reducingSymbols.size()) {// 栈顶都没有，直接结束
      break;
    }
    SyntaxDfaState *nextProductionRuleDfaState = nullptr;
    auto nextProductionRuleDfaStateIt = productionRuleDfaState->edges.find(inputReducingSymbol->reducedGrammar);
    if (nextProductionRuleDfaStateIt != productionRuleDfaState->edges.end()) {
      nextProductionRuleDfaState = nextProductionRuleDfaStateIt->second;
    }
    if (!nextProductionRuleDfaState) {// 无法按照产生式向前归约，结束
      break;
    }
    if (FaStateType::isClosingTag(nextProductionRuleDfaState->type)) {
      auto topReducingSymbolDfaStateIt = next(reducingSymbolListIt);
      const SyntaxDfaState *topReducingSymbolDfaState = (*topReducingSymbolDfaStateIt)->currentDfaState;
      SyntaxDfaState *nextDfaState = nullptr;
      auto nextDfaStateIt = topReducingSymbolDfaState->edges.find(closingProductionRule->grammar);
      if (nextDfaStateIt != topReducingSymbolDfaState->edges.end()) {
        nextDfaState = nextDfaStateIt->second;
      }
      // 连通的
      if (nextDfaState) {
        BacktrackingBottomUpBranch *newBottomUpBranch = bottomUpBranch->clone();
        newBottomUpBranch->status = BacktrackingBottomUpBranchStatus::CREATED;
        // 被归约的符号出栈，同时建立语法树孩子节点
        auto *reducingAst = new AutomataTmpAst(
            closingProductionRule->grammar, closingProductionRule->alias);
        for (int countOfUselessReducingSymbol = 1;
             countOfUselessReducingSymbol <= countOfComsumedReducingSymbol;
             countOfUselessReducingSymbol++) {
          ReducingSymbol *childReducingSymbol = newBottomUpBranch->reducingSymbols.back();
          auto *childOfReducingAst = const_cast<AutomataTmpAst *>(childReducingSymbol->astOfCurrentDfaState->clone());
          childOfReducingAst->parent = reducingAst;
          reducingAst->children.push_front(childOfReducingAst);

          newBottomUpBranch->reducingSymbols.pop_back();
          delete childReducingSymbol;
        }
        // 归约的符号
        auto *terminalReducingSymbol = new ReducingSymbol();
        terminalReducingSymbol->reducedGrammar = closingProductionRule->grammar;
        terminalReducingSymbol->endIndexOfToken = endIndexOfToken;
        terminalReducingSymbol->currentDfaState = nextDfaState;
        terminalReducingSymbol->astOfCurrentDfaState = reducingAst;
        // 归约的符号进栈
        newBottomUpBranch->reducingSymbols.push_back(terminalReducingSymbol);
        addNewBacktrackingBottomUpBranch(newBottomUpBranch);
      }
    }
    productionRuleDfaState = nextProductionRuleDfaState;
    ++reducingSymbolListIt;
  }
}

void BacktrackingBottomUpAstAutomata::shiftBottomUpBranch(BacktrackingBottomUpBranch *bottomUpBranch) {
  ReducingSymbol *topReducingSymbol = bottomUpBranch->reducingSymbols.back();
  // 将输入流定位到分支读取的位置
  tokenReducingSymbolInputStream.nextReadIndex = topReducingSymbol->endIndexOfToken + 1;
  // 移进一个token
  if (tokenReducingSymbolInputStream.hasNext()) {
    Token *token = tokenReducingSymbolInputStream.read();
    SyntaxDfaState *nextDfaState = nullptr;
    auto nextDfaStateIt = topReducingSymbol->currentDfaState->edges.find(token->terminal);
    if (nextDfaStateIt != topReducingSymbol->currentDfaState->edges.end()) {
      nextDfaState = nextDfaStateIt->second;
    }
    // 连通的
    if (nextDfaState) {
      BacktrackingBottomUpBranch *terminalBottomUpBranch = bottomUpBranch->clone();
      terminalBottomUpBranch->status = BacktrackingBottomUpBranchStatus::CREATED;
      // 归约的符号
      auto *terminalReducingSymbol = new ReducingSymbol();
      terminalReducingSymbol->reducedGrammar = token->terminal;
      terminalReducingSymbol->astOfCurrentDfaState = new AutomataTmpAst(token);
      terminalReducingSymbol->currentDfaState = nextDfaState;
      terminalReducingSymbol->endIndexOfToken = tokenReducingSymbolInputStream.nextReadIndex - 1;
      // 归约的符号进栈
      terminalBottomUpBranch->reducingSymbols.push_back(terminalReducingSymbol);
      addNewBacktrackingBottomUpBranch(terminalBottomUpBranch);
    }
  }
  bottomUpBranch->status = BacktrackingBottomUpBranchStatus::SHIFTED;
}

void BacktrackingBottomUpAstAutomata::closeBottomUpBranch(BacktrackingBottomUpBranch *bottomUpBranch) {
  ReducingSymbol *topReducingSymbol = bottomUpBranch->reducingSymbols.back();
  tokenReducingSymbolInputStream.nextReadIndex = topReducingSymbol->endIndexOfToken + 1;
  bottomUpBranch->status = BacktrackingBottomUpBranchStatus::NON_ACCEPTED;
  // 可接受状态:栈中有两个归约，栈底是基准标志，栈顶是归约结果，并且源文件输入流全部识别了
  if (tokenReducingSymbolInputStream.hasReadAll()
      && startGrammar == topReducingSymbol->reducedGrammar) {
    bottomUpBranch->status = BacktrackingBottomUpBranchStatus::ACCEPTED;
  }
}

void BacktrackingBottomUpAstAutomata::init(std::list<Token *> *sourceTokens) {
  clear();
  tokenReducingSymbolInputStream.init(sourceTokens);
  result = nullptr;

  ReducingSymbol *connectedSignOfStartGrammarReducingSymbol =
      getConnectedSignOfStartGrammarReducingSymbol();
  const SyntaxDfaState *topReducingSymbolDfaState =
      connectedSignOfStartGrammarReducingSymbol->currentDfaState;
  // 空归约
  auto &closingProductionRules = topReducingSymbolDfaState->closingProductionRules;
  for (auto closingProductionRule : closingProductionRules) {
    SyntaxDfaState *nextDfaState = nullptr;
    auto nextDfaStateIt = topReducingSymbolDfaState->edges.find(closingProductionRule->grammar);
    if (nextDfaStateIt != topReducingSymbolDfaState->edges.end()) {
      nextDfaState = nextDfaStateIt->second;
    }
    if (!nextDfaState) {
      continue;
    }
    // 连通的
    auto *nonterminalBottomUpBranch = new BacktrackingBottomUpBranch();
    // 基准标志进栈
    nonterminalBottomUpBranch->reducingSymbols.push_back(
        connectedSignOfStartGrammarReducingSymbol->clone());
    // 归约的符号
    auto *nonterminalReducingSymbol = new ReducingSymbol();
    nonterminalReducingSymbol->reducedGrammar = closingProductionRule->grammar;
    nonterminalReducingSymbol->astOfCurrentDfaState = new AutomataTmpAst(
        closingProductionRule->grammar, closingProductionRule->alias);
    nonterminalReducingSymbol->currentDfaState = nextDfaState;
    nonterminalReducingSymbol->endIndexOfToken = -1;
    // 归约的符号进栈
    nonterminalBottomUpBranch->reducingSymbols.push_back(nonterminalReducingSymbol);
    addNewBacktrackingBottomUpBranch(nonterminalBottomUpBranch);
  }
  // 移进一个token
  if (tokenReducingSymbolInputStream.hasNext()) {
    // 将输入流定位到分支读取的位置,因为是初始化，所以为0
    tokenReducingSymbolInputStream.nextReadIndex = 0;
    Token *token = tokenReducingSymbolInputStream.read();
    SyntaxDfaState *nextDfaState = nullptr;
    auto nextDfaStateIt = topReducingSymbolDfaState->edges.find(token->terminal);
    if (nextDfaStateIt != topReducingSymbolDfaState->edges.end()) {
      nextDfaState = nextDfaStateIt->second;
    }
    // 连通的
    if (nextDfaState) {
      auto *terminalBottomUpBranch = new BacktrackingBottomUpBranch();
      // 基准标志进栈
      terminalBottomUpBranch->reducingSymbols.push_back(
          connectedSignOfStartGrammarReducingSymbol->clone());
      // 归约的符号
      auto *terminalReducingSymbol = new ReducingSymbol();
      terminalReducingSymbol->reducedGrammar = token->terminal;
      terminalReducingSymbol->astOfCurrentDfaState = new AutomataTmpAst(token);
      terminalReducingSymbol->currentDfaState = nextDfaState;
      terminalReducingSymbol->endIndexOfToken = tokenReducingSymbolInputStream.nextReadIndex - 1;
      // 归约的符号进栈
      terminalBottomUpBranch->reducingSymbols.push_back(terminalReducingSymbol);
      addNewBacktrackingBottomUpBranch(terminalBottomUpBranch);
    }
  }
}

ReducingSymbol *BacktrackingBottomUpAstAutomata::getConnectedSignOfStartGrammarReducingSymbol() {
  auto *connectedSignOfStartGrammarReducingSymbol = new ReducingSymbol();
  connectedSignOfStartGrammarReducingSymbol->reducedGrammar = nullptr;
  connectedSignOfStartGrammarReducingSymbol->astOfCurrentDfaState = nullptr;
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