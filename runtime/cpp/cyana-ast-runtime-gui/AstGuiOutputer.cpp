//
// Created by tian wei jun on 2022/11/24 0024.
//

#include "AstGuiOutputer.h"
#include "GuiApi.h"

AstGuiOutputer::AstGuiOutputer(const Ast *ast) : ast(ast), stringTree(0), hasOpened(false) {
}
AstGuiOutputer::~AstGuiOutputer() {
  delete stringTree;
  stringTree = 0;
}

void AstGuiOutputer::output() {
  if (0 == ast) {
    return;
  }
  StringTree *stringTree = buildStringTree(ast);
  futureOfView =
      std::async(std::launch::async, guiapi::showViewUntilClose, stringTree);
  hasOpened = true;
}
void AstGuiOutputer::waitToClose() {
  if (hasOpened) {
    futureOfView.get();
    hasOpened = false;
  }
}

StringTree *AstGuiOutputer::buildStringTree(const Ast *ast) {
  if (ast == 0) {
    return 0;
  }
  StringTree *strTree = new StringTree(0);
  strTree->text = ast->newString();
  if (!ast->children.empty()) {
    for (Ast *astTreeChild : ast->children) {
      StringTree *strTreeChild = buildStringTree(astTreeChild);
      strTreeChild->parent = strTree;
      strTree->children->push_back(strTreeChild);
    }
  }
  return strTree;
}