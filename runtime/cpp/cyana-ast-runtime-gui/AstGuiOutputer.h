//
// Created by tian wei jun on 2022/11/24 0024.
//

#ifndef CYANA_AST__ASTGUIOUTPUTER_H_
#define CYANA_AST__ASTGUIOUTPUTER_H_

#include "CyanaAstRuntime.h"
#include "StringTree.h"
#include <future>

class AstGuiOutputer {
 public:
  explicit AstGuiOutputer(const Ast *ast);
  AstGuiOutputer(const AstGuiOutputer &astGuiOutputer) = delete;
  AstGuiOutputer(const AstGuiOutputer &&astGuiOutputer) = delete;
  ~AstGuiOutputer();

  void output();
  StringTree *buildStringTree(const Ast *argAst);
  void waitToClose();

  StringTree *stringTree;
  const Ast *ast;
  std::future<int> futureOfView;
  bool hasOpened;
};

#endif//CYANA_AST__ASTGUIOUTPUTER_H_
