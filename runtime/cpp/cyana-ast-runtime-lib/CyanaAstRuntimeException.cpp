//
// Created by tian wei jun on 2022/11/23 0023.
//

#include "CyanaAstRuntimeException.h"

#include <utility>

CyanaAstRuntimeException::CyanaAstRuntimeException(std::string  msg) noexcept : msg(std::move(msg)) {
}

CyanaAstRuntimeException::~CyanaAstRuntimeException() noexcept = default;

const char *CyanaAstRuntimeException::what() const noexcept {
  return msg.c_str();
}

CyanaAstRuntimeException::CyanaAstRuntimeException(const CyanaAstRuntimeException &ex) noexcept : msg(ex.msg) {
}
