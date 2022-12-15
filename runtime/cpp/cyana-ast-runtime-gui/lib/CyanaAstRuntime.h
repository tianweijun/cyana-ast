//
// Created by tian wei jun on 2022-12-10.
//

#ifndef CYANA_AST_RUNTIME_GUI_LIB_CYANAASTRUNTIME_H_

#include <list>
#include <map>
#include <string>
#include <exception>

enum class  TokenType : int {
  TEXT = 0,
  SKIP = 1
};

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
  explicit Grammar(const std::string *name, const GrammarType type, const GrammarAction action);
  Grammar(Grammar &grammar) = delete;
  Grammar(Grammar &&grammar) = delete;
  ~Grammar();

  const std::string *const name;
  const GrammarType type;
  const GrammarAction action;
};

class __declspec(dllimport)  Token {
 public:
  Token(const Grammar *terminal, int start, std::string text, TokenType type);
  Token(Token &token) = delete;
  Token(Token &&token) = delete;
  ~Token();
  const Token *clone() const;

  const Grammar *const terminal;
  const int start;
  const std::string text;
  const TokenType type;
};

class __declspec(dllimport) Ast {
 public:
  Ast(const Grammar *grammar, const std::string *alias);
  explicit Ast(const Token *token);
  Ast(Ast &ast) = delete;
  Ast(Ast &&ast) = delete;
  ~Ast();

  const Ast *clone() const;
  std::string *toString() const;

  const Grammar *const grammar;
  const std::string *const alias;
  // grammar.type == GrammarType.TERMINAL
  const Token *token;
  const Ast *parent;
  std::list<Ast *> children;
};

class __declspec(dllimport)  RuntimeAutomataAstApplication {
 public:
  RuntimeAutomataAstApplication();
  RuntimeAutomataAstApplication(RuntimeAutomataAstApplication &runtimeAutomataAstApplication) = delete;
  RuntimeAutomataAstApplication(RuntimeAutomataAstApplication &&runtimeAutomataAstApplication) = delete;
  ~RuntimeAutomataAstApplication();

  void setContext(const std::string *automataFilePath);
  const Ast *buildAst(const std::string *sourceCodeFilePath);

 private:
  const void *persistentAutomataAstApplication;
};


class __declspec(dllimport) CyanaAstRuntimeException : public std::exception {
 public:
  explicit CyanaAstRuntimeException(std::string msg) noexcept;
  CyanaAstRuntimeException(const CyanaAstRuntimeException &ex) noexcept;
  ~CyanaAstRuntimeException() noexcept override;

  const char *what() const noexcept override;

  const std::string msg;
};
#define CYANA_AST_RUNTIME_GUI_LIB_CYANAASTRUNTIME_H_

#endif//CYANA_AST_RUNTIME_GUI_LIB_CYANAASTRUNTIME_H_
