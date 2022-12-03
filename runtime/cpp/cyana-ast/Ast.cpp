//
// Created by tian wei jun on 2022/11/17 0017.
//

#include "Ast.h"
#include "StringUtils.h"

Ast::Ast(Grammar *grammar, std::string *alias) : grammar(grammar), alias(0),
                                                 parent(0), token(0),
                                                 children(std::list<Ast *>()) {
}
Ast::~Ast() {
  if (this->token != 0) {
    delete this->token;
    this->token = 0;
  }
  for (auto childrenIt = children.begin(); childrenIt != children.end(); childrenIt++) {
    Ast *ast = *childrenIt;
    delete ast;
    ast = 0;
  }
}

std::string *Ast::newString() {
  std::string *displayString = new std::string();
  GrammarType type = grammar->type;
  if (type == GrammarType::TERMINAL) {
    *displayString = token->text;
  }
  if (type == GrammarType::NONTERMINAL) {
    if (StringUtils::isNotBlank(const_cast<std::string *>(alias))) {
      *displayString = *(grammar->name) + "[" + *alias + "]";
    } else {
      *displayString = *(grammar->name);
    }
  }
  return displayString;
}
