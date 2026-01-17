#pragma once
#include <string>

enum struct Type {
  IDENTIFIER,
  NUMBER, STRING, BOOL, NIL,
  PLUS, MINUS, MUL, DIV, REM, POW,
  PLUS_ASN, MUNIS_ASN, MUL_ASN, DIV_ASN, REM_ASN, POW_ASN,
  ASSIGN,
  EQUAL, NOT_EQUAL, LESS, GREATER, LESS_EQUAL, GREATER_EQUAL,
  AND, NOT, OR,
  LEFT_BRACKET, RIGHT_BRACKET, LEFT_PAREN, RIGHT_PAREN,
  IF, ELSE, THEN, END, FOR, IN, WHILE, BREAK, CONTINUE, 
  COMMA, COLON, EOL, EOFF,
  FUNCTION, RETURN
};

class Token {
 public:
  Token() {}
  Token(Type type, std::string lexeme, unsigned line, unsigned pos)
      : type_(type), lexeme_(lexeme), line_(line),  pos_(pos) {}

  Type GetType() const;
  std::string GetLexeme() const;
  unsigned GetLine() const;
  unsigned GetPos() const;

 private:
  Type type_;
  std::string lexeme_;
  unsigned line_;
  unsigned pos_;
};
