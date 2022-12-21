//
// Created by tian wei jun on 2022/11/24 0024.
//

#include "AstGuiOutputer.h"
#include "lib/CyanaAstGuiLib.h"

AstGuiOutputer::AstGuiOutputer(const Ast *ast) : ast(ast), stringTree(0), hasOpened(false) {
}
AstGuiOutputer::~AstGuiOutputer() {
  delete stringTree;
  stringTree = nullptr;
}

void AstGuiOutputer::output() {
  if (!ast) {
    return;
  }
  StringTree *ast2StringTree = buildStringTree(ast);
  futureOfView =
      std::async(std::launch::async, CyanaAstGuiLib::showViewUntilClose, ast2StringTree);
  hasOpened = true;
}
void AstGuiOutputer::waitToClose() {
  if (hasOpened) {
    futureOfView.get();
    hasOpened = false;
  }
}

StringTree *AstGuiOutputer::buildStringTree(const Ast *argAst) {
  if (!argAst) {
    return nullptr;
  }
  auto *strTree = new StringTree(nullptr);
  strTree->text = argAst->toString();
  if (!argAst->children.empty()) {
    for (Ast *astTreeChild : argAst->children) {
      StringTree *strTreeChild = buildStringTree(astTreeChild);
      strTreeChild->parent = strTree;
      strTree->children->push_back(strTreeChild);
    }
  }
  return strTree;
}