//
// Created by tian wei jun on 2022/11/17 0017.
//

#include "Ast.h"
#include "CyanaAstRuntimeException.h"
#include "StringUtils.h"

Ast::Ast(Grammar *grammar, std::string *alias) : grammar(grammar), alias(alias),
                                                 parent(0), token(0),
                                                 children(std::list<Ast *>()) {
}

Ast::Ast(ProductionRule *productionRule) : Ast(productionRule->grammar, productionRule->alias) {
}

Ast::Ast(Token *token) : Ast(const_cast<Grammar *>(token->terminal), 0) {
  this->token = token;
}

Ast::~Ast() {
  //grammar delete by PersistentData.grammars
  //alias delete by PersistentData.stringPool
  //parent delete by itself
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

const Ast *Ast::clone() const {
  if (0 != parent) {
    throw new CyanaAstRuntimeException("parent of ast must be null in clone of Ast");
  }
  AstCloner astCloner(this);
  return astCloner.clone();
}

AstCloner::AstCloner(const Ast *ast) : source(ast),
                                       sourceDestAstMap(std::map<Ast *, Ast *>()),
                                       sourceAsts(std::list<Ast *>()) {
  setSourceAsts(ast);
}

AstCloner::~AstCloner() {
  // source delete by user
}

Ast *AstCloner::clone() {
  mapSourceDestAst();
  cloneAst();
}

void AstCloner::cloneAst() {
  for (auto sourceAstsIt = sourceAsts.begin(); sourceAstsIt != sourceAsts.end(); sourceAstsIt++) {
    Ast *sourceAst = *sourceAstsIt;
    Ast *destAst = sourceDestAstMap.find(sourceAst)->second;
    destAst->token = sourceAst->token->clone();
    if (0 != sourceAst->parent) {
      destAst->parent = sourceDestAstMap.find(sourceAst->parent)->second;
    }
    auto childrenOfSourceAst = sourceAst->children;
    auto childrenOfDestAst = destAst->children;
    for (auto childrenOfSourceAstIt = childrenOfSourceAst.begin(); childrenOfSourceAstIt != childrenOfSourceAst.end(); childrenOfSourceAstIt++) {
      Ast *childOfSourceAst = *childrenOfSourceAstIt;
      childrenOfDestAst.push_back(sourceDestAstMap.find(childOfSourceAst)->second);
    }
  }
}

void AstCloner::mapSourceDestAst() {
  sourceDestAstMap.clear();
  for (auto sourceAstsIt = sourceAsts.begin(); sourceAstsIt != sourceAsts.end(); sourceAstsIt++) {
    Ast *sourceAst = *sourceAstsIt;
    Ast *ast = new Ast(const_cast<Grammar *>(sourceAst->grammar), const_cast<std::string *>(sourceAst->alias));
    std::pair<Ast *, Ast *> keyValue(sourceAst, ast);
    sourceDestAstMap.insert(keyValue);
  }
}

void AstCloner::setSourceAsts(const Ast *ast) {
  sourceAsts.push_back(const_cast<Ast *>(ast));
  auto children = ast->children;
  for (auto childrenIt = children.begin(); childrenIt != children.end(); childrenIt++) {
    Ast *child = *childrenIt;
    setSourceAsts(child);
  }
}
