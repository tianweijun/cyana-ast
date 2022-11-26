//
// Created by tian wei jun on 2022/11/17 0017.
//

#include "Ast.h"
#include "AstType.h"
#include "StringUtils.h"

Ast::Ast() : token(0), parent(0), type(AstType::TERMINAL) {
}

std::string* Ast:: newString(){
  std::string *displayString = new std::string();
  if (type == AstType::TERMINAL) {
    *displayString = token->text;
  }
  if (type == AstType::NONTERMINAL) {
    if (StringUtils::isNotBlank(alias)) {
      *displayString = name + "[" + alias + "]";
    } else {
      *displayString = name;
    }
  }
  return displayString;
}
