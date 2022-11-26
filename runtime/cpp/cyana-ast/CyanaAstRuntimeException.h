//
// Created by tian wei jun on 2022/11/23 0023.
//

#ifndef CYANA_AST_RUNTIME__CYANAASTRUNTIMEEXCEPTION_H_
#define CYANA_AST_RUNTIME__CYANAASTRUNTIMEEXCEPTION_H_

#include <exception>
#include <string>

class CyanaAstRuntimeException : public std::exception {
 public:
  CyanaAstRuntimeException(std::string msg) noexcept;
  ~CyanaAstRuntimeException() noexcept;

  const char *what() const noexcept;

  std::string msg;
};

#endif//CYANA_AST_RUNTIME__CYANAASTRUNTIMEEXCEPTION_H_
