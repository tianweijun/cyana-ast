//
// Created by tian wei jun on 2022-12-10.
//

#ifndef CYANA_AST_RUNTIME_GUI_LIB_CYANAASTRUNTIME_H_

#include <list>
#include <map>
#include <string>
#include <exception>

enum class  GrammarType : int {
  TERMINAL_FRAGMENT = 0,
  TERMINAL = 1,
  NONTERMINAL = 2
};

enum  class GrammarAction : int {
  TEXT = 0,
  SKIP = 1
};

class __declspec(dllimport) Grammar {
 public:
  explicit Grammar(std::string *name);
  ~Grammar();

  const std::string *const name;
  GrammarType type;
  GrammarAction action;
};

enum class  TokenType : int {
  TEXT = 0,
  SKIP = 1
};

class __declspec(dllimport)  Token {
 public:
  Token(Grammar *terminal, int start);
  ~Token();
  const Token *clone() const;

  int start;
  std::string text;
  const Grammar *const terminal;
  TokenType type;
};

class __declspec(dllimport) Ast {
 public:
  Ast(Grammar *grammar, std::string *alias);
  explicit Ast(Token *token);
  ~Ast();

  const Ast *clone() const;
  std::string *newString() const;

  const Grammar *const grammar;
  const std::string *const alias;
  // grammar.type == GrammarType.TERMINAL
  const Token *token;
  Ast *parent;
  std::list<Ast *> children;
};

class __declspec(dllimport)  RuntimeAutomataAstApplication {
 public:
  RuntimeAutomataAstApplication();
  ~RuntimeAutomataAstApplication();

  void setContext(const std::string *automataFilePath);
  const Ast *buildAst(const std::string *sourceCodeFilePath);
 private:
  void *persistentAutomataAstApplication;
};


class __declspec(dllimport) CyanaAstRuntimeException : public std::exception {
 public:
  explicit CyanaAstRuntimeException(std::string msg) noexcept;
  CyanaAstRuntimeException(const CyanaAstRuntimeException &ex) noexcept;
  ~CyanaAstRuntimeException() noexcept override;

  const char *what() const noexcept override;

  std::string msg;
};
#define CYANA_AST_RUNTIME_GUI_LIB_CYANAASTRUNTIME_H_

#endif//CYANA_AST_RUNTIME_GUI_LIB_CYANAASTRUNTIME_H_
