//
// Created by tian wei jun on 2022/11/17 0017.
//

#include "Token.h"

Token::Token(Grammar *terminal, int start) : start(start), terminal(terminal), type(TokenType::TEXT) {
}

Token::~Token() {
  //terminal delete by PersistentData.grammars
}

const Token *Token::clone() const {
  Token *token = new Token(const_cast<Grammar *>(this->terminal), this->start);
  token->text = this->text;
  token->type = this->type;
  return token;
}