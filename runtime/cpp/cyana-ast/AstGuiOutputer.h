//
// Created by tian wei jun on 2022/11/24 0024.
//

#ifndef CYANA_AST__ASTGUIOUTPUTER_H_
#define CYANA_AST__ASTGUIOUTPUTER_H_

#include "Ast.h"
#include "StringTree.h"
#include <future>

class AstGuiOutputer {
 public:
  AstGuiOutputer(Ast *ast);
  ~AstGuiOutputer();

  void output();
  StringTree *buildStringTree(Ast *ast);
  void waitToClose();

  StringTree *stringTree;
  Ast *ast;
  std::future<int> futureOfView;
  bool hasOpened;
};

#endif//CYANA_AST__ASTGUIOUTPUTER_H_
