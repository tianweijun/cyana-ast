//
// Created by tian wei jun on 2022/11/23 0023.
//

#ifndef CYANA_AST_RUNTIME__CYANAASTRUNTIMEEXCEPTION_H_
#define CYANA_AST_RUNTIME__CYANAASTRUNTIMEEXCEPTION_H_

#include <exception>
#include <string>

class __declspec(dllexport) CyanaAstRuntimeException : public std::exception {
 public:
  explicit CyanaAstRuntimeException(std::string  msg) noexcept;
  CyanaAstRuntimeException(const CyanaAstRuntimeException &ex) noexcept;
  ~CyanaAstRuntimeException() noexcept override;

  const char *what() const noexcept override;

  const std::string msg;
};

#endif//CYANA_AST_RUNTIME__CYANAASTRUNTIMEEXCEPTION_H_
