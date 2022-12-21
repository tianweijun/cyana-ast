//
// Created by tian wei jun on 2022/11/23 0023.
//

#ifndef CYANA_AST_RUNTIME__CYANAASTRUNTIMEEXCEPTION_H_
#define CYANA_AST_RUNTIME__CYANAASTRUNTIMEEXCEPTION_H_

#include <exception>
#include <list>
#include <string>

enum class CyanaAstRuntimeExceptionCode : int {
  LOGIC_ERROR = 0,
  IO_ERROR,
  INVALID_ARGUMENT
};

class __declspec(dllexport) CyanaAstRuntimeException : public std::exception {
 public:
  explicit CyanaAstRuntimeException(CyanaAstRuntimeExceptionCode code, std::string msg) noexcept;
  explicit CyanaAstRuntimeException(CyanaAstRuntimeExceptionCode code, const char *msg) noexcept;
  CyanaAstRuntimeException(const CyanaAstRuntimeException &ex);
  ~CyanaAstRuntimeException() noexcept override;

  const char *what() const noexcept override;

  const CyanaAstRuntimeExceptionCode code;
  const std::string msg;
};

class __declspec(dllexport) HandlerExceptionResolver {
 private:
  HandlerExceptionResolver();
  ~HandlerExceptionResolver();

 public:
  HandlerExceptionResolver(HandlerExceptionResolver &ExResolver) = delete;
  HandlerExceptionResolver(HandlerExceptionResolver &&ExResolver) = delete;

 public:
  static void throwException(const CyanaAstRuntimeException *ex);
  static void clearExceptions();
  static bool hasThrewException();
  static const std::list<CyanaAstRuntimeException *> *getExceptions();
  static void destroy();

 private:
  static std::list<CyanaAstRuntimeException *> *exceptions;
};

#endif//CYANA_AST_RUNTIME__CYANAASTRUNTIMEEXCEPTION_H_
