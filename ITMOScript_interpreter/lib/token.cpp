#include "token.h"

Type Token::GetType() const {
  return type_;
}

std::string Token::GetLexeme() const {
  return lexeme_;
}

unsigned Token::GetLine() const {
  return line_;
}

unsigned Token::GetPos() const {
  return pos_;
}
