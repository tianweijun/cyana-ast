//
// Created by tian wei jun on 2022/11/24 0024.
//

#ifndef CYANA_AST__ASTGUIOUTPUTER_H_
#define CYANA_AST__ASTGUIOUTPUTER_H_

#include "Ast.h"
#include "StringTree.h"

class AstGuiOutputer {
 public:
  void output(Ast* ast);
  void output(StringTree* stringTree);
  StringTree* buildStringTree(Ast* ast);
};

#endif//CYANA_AST__ASTGUIOUTPUTER_H_
