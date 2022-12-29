//
// Created by tian wei jun on 2022-12-10.
//

#ifndef CYANA_AST_RUNTIME_GUI_LIB_CYANAASTRUNTIME_H_

#include <exception>
#include <list>
#include <map>
#include <string>

enum class TokenType : int {
  TEXT = 0,
  SKIP = 1
};

enum class GrammarType : int {
  TERMINAL_FRAGMENT = 0,
  TERMINAL = 1,
  NONTERMINAL = 2
};

enum class GrammarAction : int {
  TEXT = 0,
  SKIP = 1
};

class __declspec(dllimport) Grammar {
 public:
  explicit Grammar(const std::string *name, const GrammarType type, const GrammarAction action);
  Grammar(const Grammar &grammar) = delete;
  Grammar(const Grammar &&grammar) = delete;
  ~Grammar();

  const std::string *const name;
  const GrammarType type;
  const GrammarAction action;
};

class __declspec(dllimport) Token {
 public:
  Token(const Grammar *terminal, int start, std::string text, TokenType type);
  Token(const Token &token) = delete;
  Token(const Token &&token) = delete;
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
  Ast(const Ast &ast) = delete;
  Ast(const Ast &&ast) = delete;
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

class __declspec(dllimport) RuntimeAutomataAstApplication {
 public:
  RuntimeAutomataAstApplication();
  RuntimeAutomataAstApplication(const RuntimeAutomataAstApplication &runtimeAutomataAstApplication) = delete;
  RuntimeAutomataAstApplication(const RuntimeAutomataAstApplication &&runtimeAutomataAstApplication) = delete;
  ~RuntimeAutomataAstApplication();

  void setContext(const std::string *automataFilePath);
  const Ast *buildAst(const std::string *sourceCodeFilePath);

 private:
  const void *persistentAutomataAstApplication;
};


enum class CyanaAstRuntimeExceptionCode : int {
  LOGIC_ERROR = 0,
  IO_ERROR,
  INVALID_ARGUMENT
};

class __declspec(dllimport) CyanaAstRuntimeException : public std::exception {
 public:
  explicit CyanaAstRuntimeException(CyanaAstRuntimeExceptionCode code, std::string msg) noexcept;
  explicit CyanaAstRuntimeException(CyanaAstRuntimeExceptionCode code, const char *msg) noexcept;
  CyanaAstRuntimeException(const CyanaAstRuntimeException &ex);
  ~CyanaAstRuntimeException() noexcept override;

  const char *what() const noexcept override;

  const CyanaAstRuntimeExceptionCode code;
  const std::string msg;
};

class __declspec(dllimport) CyanaAstRuntimeExceptionResolver {
 private:
  CyanaAstRuntimeExceptionResolver();
  ~CyanaAstRuntimeExceptionResolver();

 public:
  CyanaAstRuntimeExceptionResolver(CyanaAstRuntimeExceptionResolver &ExResolver) = delete;
  CyanaAstRuntimeExceptionResolver(CyanaAstRuntimeExceptionResolver &&ExResolver) = delete;

 public:
  static void throwException(const CyanaAstRuntimeException *ex);
  static void clearExceptions();
  static bool hasThrewException();
  static const std::list<CyanaAstRuntimeException *> *getExceptions();
  static void destroy();

 private:
  static void *exceptions;
};

#define CYANA_AST_RUNTIME_GUI_LIB_CYANAASTRUNTIME_H_

#endif//CYANA_AST_RUNTIME_GUI_LIB_CYANAASTRUNTIME_H_
