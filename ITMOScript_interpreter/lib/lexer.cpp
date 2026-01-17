#include "lexer.h"

void Lexer::Tokenize() {
  Token new_token = IdentifyToken();
  while (new_token.GetType() != Type::EOFF) {
    tokens_.emplace_back(new_token);
    new_token = IdentifyToken();
  }
  tokens_.emplace_back(new_token);
}

std::vector<Token> Lexer::GetTokens() { return tokens_; }

void Lexer::update() {
  if (code_.get(current_)) {
    if (current_ == '\n') {
      pos_ = 0;
      ++line_;
    } else {
      ++pos_;
    }
  } else {
    current_ = '\0';
  }
}

Token Lexer::ReadNumber() {
  while (current_ == ' ') {
    update();
  }
  std::string number = "";
  while (isdigit(current_) || current_ == 'e' || current_ == 'E' ||
         current_ == '.' || current_ == '-' || current_ == '+') {
    number += current_;
    previous_ = current_;
    update();
  }
  return Token{Type::NUMBER, number, line_, pos_};
}

Token Lexer::ReadString() {
  while (current_ == ' ') {
    update();
  }
  std::string str = "";
  while (current_ != '\"') {
    str += current_;
    previous_ = current_;
    update();
  }
  return Token{Type::STRING, str, line_, pos_};
}

Token Lexer::ReadWord() {
  while (current_ == ' ') {
    update();
  }
  std::string word = "";
  word += current_;
  update();
  while (isalpha(current_) || current_ == '_' || isdigit(current_)) {
    word += current_;
    previous_ = current_;
    update();
  }

  if (word == "true" || word == "false") {
    return Token{Type::BOOL, word, line_, pos_};
  }

  if (token_map_.count(word)) {
    return Token{token_map_[word], word, line_, pos_};
  }

  return Token{Type::IDENTIFIER, word, line_, pos_};
}

Token Lexer::IdentifyToken() {
  while (current_ == ' ') {
    update();
  }

  if (current_ == '\0') {
    return Token{Type::EOFF, std::string("\0"), line_, pos_};
  }

  switch (current_) {
    case '+':
      previous_ = current_;
      update();
      if (current_ == '=') {  
        previous_ = current_;
        update();
        return Token{Type::PLUS_ASN, std::string("+="), line_, pos_};
      } else {
        return Token{Type::PLUS, std::string("+"), line_, pos_};
      }

    case '-':
      previous_ = current_;
      update();
      if (current_ == '=') {
        update();
        return Token{Type::MUNIS_ASN, std::string("-="), line_, pos_};
      } else {
        return Token{Type::MINUS, std::string("-"), line_, pos_};
      }

    case '*':
      previous_ = current_;
      update();
      if (current_ == '=') {
        previous_ = current_;
        update();
        return Token{Type::MUL_ASN, std::string("*="), line_, pos_};
      } else {
        return Token{Type::MUL, std::string("*"), line_, pos_};
      }

    case '/':
      previous_ = current_;
      update();
      if (current_ == '=') {
        previous_ = current_;
        update();
        return Token{Type::DIV_ASN, std::string("/="), line_, pos_};
      } else {
        return Token{Type::DIV, std::string("/"), line_, pos_};
      }

    case '%':
      previous_ = current_;
      update();
      if (current_ == '=') {
        previous_ = current_;
        update();
        return Token{Type::REM_ASN, std::string("%="), line_, pos_};
      } else {
        return Token{Type::REM, std::string("%"), line_, pos_};
      }

    case '^':
      previous_ = current_;
      update();
      if (current_ == '=') {
        previous_ = current_;
        update();
        return Token{Type::POW_ASN, std::string("^="), line_, pos_};
      } else {
        return Token{Type::POW, std::string("^"), line_, pos_};
      }

    case '=':
      previous_ = current_;
      update();
      if (current_ == '=') {
        previous_ = current_;
        update();
        return Token{Type::EQUAL, std::string("=="), line_, pos_};
      } else {
        return Token{Type::ASSIGN, std::string("="), line_, pos_};
      }

    case '!':
      previous_ = current_;
      update();
      if (current_ == '=') {
        previous_ = current_;
        update();
        return Token{Type::NOT_EQUAL, std::string("!="), line_, pos_};
      } else {
        throw std::runtime_error("Unknown operator in line" +
                                 std::to_string(line_) + "\n");
      }

    case '<':
      previous_ = current_;
      update();
      if (current_ == '=') {
        previous_ = current_;
        update();
        return Token{Type::LESS_EQUAL, std::string("<="), line_, pos_};
      } else {
        return Token{Type::LESS, std::string("<"), line_, pos_};
      }

    case '>':
      previous_ = current_;
      update();
      if (current_ == '=') {
        previous_ = current_;
        update();
        return Token{Type::LESS_EQUAL, std::string(">="), line_, pos_};
      } else {
        return Token{Type::LESS, std::string(">"), line_, pos_};
      }

    case '\"':
      previous_ = current_;
      update();
      return ReadString();

    case '\n':
      previous_ = current_;
      update();
      return Token{Type::EOL, "\n", line_, pos_};
    case '(':
      update();
      return Token{Type::LEFT_PAREN, "(", line_, pos_};
    case ')':
      update();
      return Token{Type::RIGHT_PAREN, ")", line_, pos_};
    case '[':
      update();
      return Token{Type::LEFT_BRACKET, "[", line_, pos_};
    case ']':
      update();
      return Token{Type::RIGHT_BRACKET, "]", line_, pos_};
    case ':':
      update();
      return Token{Type::COLON, ":", line_, pos_};
    case ',':
      update();
      return Token{Type::COMMA, ",", line_, pos_};
  }

  if (isalpha(current_)) {
    previous_ = current_;
    return ReadWord();
  }

  if (isdigit(current_)) {
    previous_ = current_;
    return ReadNumber();
  }

  throw std::runtime_error("Unknown lexeme in string" + std::to_string(line_) +
                           "\n");
}
