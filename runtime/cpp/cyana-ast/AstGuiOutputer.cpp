//
// Created by tian wei jun on 2022/11/24 0024.
//

#include "AstGuiOutputer.h"
#include "lib/CyanaAstGuiLib.h"

#include <future>

void AstGuiOutputer::output(Ast* ast){
  if(0==ast){
    return;
  }
  StringTree* stringTree = buildStringTree(ast);
  output(stringTree);
}

StringTree* AstGuiOutputer::buildStringTree(Ast* ast){
  StringTree* strTree = new StringTree(0);
  strTree->text = ast->newString();
  if (!ast->children.empty()) {
    for (Ast* astTreeChild : ast->children) {
      StringTree* strTreeChild = buildStringTree(astTreeChild);
      strTreeChild->parent = strTree;
      strTree->children->push_back(strTreeChild);
    }
  }
  return strTree;
}

void AstGuiOutputer::output(StringTree* stringTree){
  std::future<int> futureOfView =
      std::async(std::launch::async, CyanaAstGuiLib::showViewUntilClose, stringTree);
  futureOfView.get();

  delete stringTree;
  stringTree = 0;
}