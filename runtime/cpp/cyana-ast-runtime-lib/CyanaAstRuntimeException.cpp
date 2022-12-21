//
// Created by tian wei jun on 2022/11/23 0023.
//

#include "CyanaAstRuntimeException.h"

#include <utility>

CyanaAstRuntimeException::CyanaAstRuntimeException(CyanaAstRuntimeExceptionCode code, std::string msg) noexcept
    : code(code), msg(std::move(msg)) {
}

CyanaAstRuntimeException::CyanaAstRuntimeException(CyanaAstRuntimeExceptionCode code, const char *msg) noexcept
    : code(code), msg(msg) {
}

CyanaAstRuntimeException::CyanaAstRuntimeException(const CyanaAstRuntimeException &ex) noexcept
    : code(ex.code), msg(ex.msg) {
}

CyanaAstRuntimeException::~CyanaAstRuntimeException() noexcept = default;

const char *CyanaAstRuntimeException::what() const noexcept {
  return msg.c_str();
}

std::list<CyanaAstRuntimeException *> *HandlerExceptionResolver::exceptions = new std::list<CyanaAstRuntimeException *>();

HandlerExceptionResolver::HandlerExceptionResolver() = default;

HandlerExceptionResolver::~HandlerExceptionResolver() = default;

void HandlerExceptionResolver::throwException(const CyanaAstRuntimeException *ex) {
  exceptions->push_back(const_cast<CyanaAstRuntimeException *>(ex));
}

void HandlerExceptionResolver::clearExceptions() {
  for (auto &exception : *exceptions) {
    delete exception;
  }
  exceptions->clear();
}

bool HandlerExceptionResolver::hasThrewException() {
  return !exceptions->empty();
}

const std::list<CyanaAstRuntimeException *> *HandlerExceptionResolver::getExceptions() {
  return exceptions;
}

void HandlerExceptionResolver::destroy() {
  clearExceptions();
  delete exceptions;
  exceptions = nullptr;
}
