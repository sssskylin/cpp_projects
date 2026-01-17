#pragma once
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

#include "token.h"

class Lexer {
 public:
  Lexer(std::istream& code) : code_(code), line_(0), pos_(0) {
    update();
  }

  void Tokenize();
  std::vector<Token> GetTokens();

 private:
  void update();
  Token ReadNumber();
  Token ReadString();
  Token ReadWord();
  Token IdentifyToken();

  std::istream& code_;
  unsigned line_;
  unsigned pos_;
  char current_;
  char previous_;
  std::vector<Token> tokens_;
  std::map<std::string, Type> token_map_{
      {"nil", Type::NIL},        {"and", Type::AND},
      {"not", Type::NOT},        {"or", Type::OR},
      {"if", Type::IF},          {"then", Type::THEN},
      {"else", Type::ELSE},      {"end", Type::END},
      {"for", Type::FOR},        {"in", Type::IN},
      {"while", Type::WHILE},    {"function", Type::FUNCTION},
      {"return", Type::RETURN}};
};
