//
// Created by tian wei jun on 2022/11/17 0017.
//

#include "Ast.h"
#include "CyanaAstRuntimeException.h"
#include "StringUtils.h"

Ast::Ast(const Grammar *grammar, const std::string *alias) : grammar(grammar), alias(alias),
                                                             parent(nullptr), token(nullptr),
                                                             children(std::list<Ast *>()) {
}

Ast::Ast(const Token *token) : grammar(const_cast<Grammar *>(token->terminal)), alias(nullptr),
                               parent(nullptr), token(nullptr),
                               children(std::list<Ast *>()) {
  this->token = token->clone();
}

Ast::~Ast() {
  //grammar delete by PersistentData.grammars
  //alias delete by PersistentData.stringPool
  //parent delete by itself
  if (this->token) {
    delete this->token;
    this->token = nullptr;
  }
  for (auto ast : children) {
    delete ast;
    ast = nullptr;
  }
}

std::string *Ast::toString() const {
  auto *displayString = new std::string();
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
  if (!parent) {
    HandlerExceptionResolver::throwException(
        new CyanaAstRuntimeException( CyanaAstRuntimeExceptionCode::LOGIC_ERROR,
                                     "parent of ast must be null in clone of Ast"));
  }
  AstCloner astCloner(this);
  return astCloner.clone();
}

AstCloner::AstCloner(const Ast *ast) : source(ast),
                                       sourceDestAstMap(std::map<const Ast *, Ast *>()),
                                       sourceAsts(std::list<Ast *>()) {
  setSourceAsts(ast);
}

AstCloner::~AstCloner() = default;

Ast *AstCloner::clone() {
  mapSourceDestAst();
  cloneAst();
  return sourceDestAstMap.find(const_cast<Ast *>(source))->second;
}

void AstCloner::cloneAst() {
  for (auto sourceAst : sourceAsts) {
    Ast *destAst = sourceDestAstMap.find(sourceAst)->second;
    if (sourceAst->token) {
      destAst->token = sourceAst->token->clone();
    }
    if (sourceAst->parent) {
      destAst->parent = sourceDestAstMap.find(sourceAst->parent)->second;
    }
    auto &childrenOfSourceAst = sourceAst->children;
    auto &childrenOfDestAst = destAst->children;
    for (auto childOfSourceAst : childrenOfSourceAst) {
      childrenOfDestAst.push_back(sourceDestAstMap.find(childOfSourceAst)->second);
    }
  }
}

void AstCloner::mapSourceDestAst() {
  sourceDestAstMap.clear();
  for (auto sourceAst : sourceAsts) {
    Ast *ast = new Ast(const_cast<Grammar *>(sourceAst->grammar), const_cast<std::string *>(sourceAst->alias));
    std::pair<Ast *, Ast *> keyValue(sourceAst, ast);
    sourceDestAstMap.insert(keyValue);
  }
}

void AstCloner::setSourceAsts(const Ast *ast) {
  sourceAsts.push_back(const_cast<Ast *>(ast));
  auto &children = ast->children;
  for (auto child : children) {
    setSourceAsts(child);
  }
}
