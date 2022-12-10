//
// Created by tian wei jun on 2022/11/17 0017.
//

#include "Token.h"

#include <utility>

Token::Token(const Grammar *terminal, int start, std::string text, TokenType type)
    : terminal(terminal), start(start), text(std::move(text)), type(type) {
}

//terminal delete by PersistentData.grammars
Token::~Token() = default;

const Token *Token::clone() const {
  auto *token = new Token(const_cast<Grammar *>(this->terminal),
                          this->start, this->text, this->type);

  return token;
}