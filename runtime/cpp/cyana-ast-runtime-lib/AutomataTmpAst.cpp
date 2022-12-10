//
// Created by tian wei jun on 2022/12/8 0008.
//

#include "AutomataTmpAst.h"
#include "CyanaAstRuntimeException.h"

AutomataTmpAst::AutomataTmpAst(const Grammar *grammar, const std::string *alias) : grammar(grammar), alias(alias),
                                                                                   parent(nullptr), token(nullptr),
                                                                                   children(std::list<AutomataTmpAst *>()) {
}

AutomataTmpAst::AutomataTmpAst(const Token *token) : grammar(const_cast<Grammar *>(token->terminal)), alias(nullptr),
                                                     parent(nullptr), token(token),
                                                     children(std::list<AutomataTmpAst *>()) {
}

AutomataTmpAst::~AutomataTmpAst() {
  //grammar delete by PersistentData.grammars
  //alias delete by PersistentData.stringPool
  //parent delete by itself
  // token did not need to delete
  for (auto ast : children) {
    delete ast;
    ast = nullptr;
  }
}

const AutomataTmpAst *AutomataTmpAst::clone() const {
  if (parent) {
    throw CyanaAstRuntimeException("parent of ast must be null in clone of AutomataTmpAst");
  }
  AutomataTmpAstCloner astCloner(this);
  return astCloner.clone();
}

AutomataTmpAstCloner::AutomataTmpAstCloner(const AutomataTmpAst *ast) : source(ast),
                                                                        sourceDestAstMap(std::map<const AutomataTmpAst *, AutomataTmpAst *>()),
                                                                        sourceAsts(std::list<AutomataTmpAst *>()) {
  setSourceAsts(ast);
}

// source delete by user
AutomataTmpAstCloner::~AutomataTmpAstCloner() = default;

AutomataTmpAst *AutomataTmpAstCloner::clone() {
  mapSourceDestAst();
  cloneAst();
  return sourceDestAstMap.find(const_cast<AutomataTmpAst *>(source))->second;
}

void AutomataTmpAstCloner::cloneAst() {
  for (auto sourceAst : sourceAsts) {
    AutomataTmpAst *destAst = sourceDestAstMap.find(sourceAst)->second;
    destAst->token = sourceAst->token;
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

void AutomataTmpAstCloner::mapSourceDestAst() {
  sourceDestAstMap.clear();
  for (auto sourceAst : sourceAsts) {
    auto *ast = new AutomataTmpAst(const_cast<Grammar *>(sourceAst->grammar), const_cast<std::string *>(sourceAst->alias));
    std::pair<AutomataTmpAst *, AutomataTmpAst *> keyValue(sourceAst, ast);
    sourceDestAstMap.insert(keyValue);
  }
}

void AutomataTmpAstCloner::setSourceAsts(const AutomataTmpAst *ast) {
  sourceAsts.push_back(const_cast<AutomataTmpAst *>(ast));
  auto &children = ast->children;
  for (auto child : children) {
    setSourceAsts(child);
  }
}
//-------------------AutomataTmpAst2AstConverter start---------------------

AutomataTmpAst2AstConverter::AutomataTmpAst2AstConverter(const AutomataTmpAst *ast) : source(ast),
                                                                                      sourceDestAstMap(std::map<const AutomataTmpAst *, Ast *>()),
                                                                                      sourceAsts(std::list<AutomataTmpAst *>()) {
  setSourceAsts(ast);
}

// source delete by user
AutomataTmpAst2AstConverter::~AutomataTmpAst2AstConverter() = default;

Ast *AutomataTmpAst2AstConverter::convert() {
  mapSourceDestAst();
  cloneAst();
  return sourceDestAstMap.find(const_cast<AutomataTmpAst *>(source))->second;
}

void AutomataTmpAst2AstConverter::cloneAst() {
  for (auto sourceAst : sourceAsts) {
    Ast *destAst = sourceDestAstMap.find(sourceAst)->second;
    destAst->token = sourceAst->token;
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

void AutomataTmpAst2AstConverter::mapSourceDestAst() {
  sourceDestAstMap.clear();
  for (auto sourceAst : sourceAsts) {
    Ast *ast = new Ast(const_cast<Grammar *>(sourceAst->grammar), const_cast<std::string *>(sourceAst->alias));
    std::pair<AutomataTmpAst *, Ast *> keyValue(sourceAst, ast);
    sourceDestAstMap.insert(keyValue);
  }
}

void AutomataTmpAst2AstConverter::setSourceAsts(const AutomataTmpAst *ast) {
  sourceAsts.push_back(const_cast<AutomataTmpAst *>(ast));
  auto &children = ast->children;
  for (auto child : children) {
    setSourceAsts(child);
  }
}