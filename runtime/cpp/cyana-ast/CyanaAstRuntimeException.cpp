//
// Created by tian wei jun on 2022/11/23 0023.
//

#include "CyanaAstRuntimeException.h"

CyanaAstRuntimeException::CyanaAstRuntimeException(std::string msg) noexcept : msg(msg) {
}

CyanaAstRuntimeException::~CyanaAstRuntimeException() noexcept {
}

const char *CyanaAstRuntimeException::what() const noexcept {
  return msg.c_str();
}
