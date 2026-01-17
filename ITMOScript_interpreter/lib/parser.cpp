#include "parser.h"
#include <iostream>

template <typename... Types>
bool Parser::Match(Types... types) {
  if ((Check(types) || ...)) {
    Update();
    return true;
  }
  return false;
}

template <typename... Types>
bool Parser::MatchAll(Types... types) {
  if ((Check(types) && ...) &&
      (current_ + sizeof...(types) <= tokens_.size())) {
    for (size_t i = 0; i < sizeof...(types); ++i) {
      Update();
    }
    return true;
  }
  return false;
}

bool Parser::Check(Type type) const {
  return current_ < tokens_.size() && tokens_[current_].GetType() == type;
}

const Token& Parser::Update() {
  if (current_ < tokens_.size()) {
    ++current_;
  }
  return tokens_[current_];
}

template <typename... Types>
void Parser::Require(Types... types) {
  if (!Match(types...)) {
    throw std::runtime_error("Parser error in position " +
                             std::to_string(current_) +
                             ": other token expected");
  }
}

template <typename... Types>
void Parser::RequireAll(Types... types) {
  if (!MatchAll(types...)) {
    throw std::runtime_error("Parser error in position " +
                             std::to_string(current_) +
                             ": other token expected");
  }
}

std::unique_ptr<Statements> Parser::ParseCode() {
  std::unique_ptr<Statements> root = std::make_unique<Statements>();\
  while (current_ < tokens_.size()) {
    root->AddNode(ParseStatement());
  }
  return root;
}

std::unique_ptr<Expression> Parser::ParseStatement() {
  Token current_token = tokens_[current_];
  switch (current_token.GetType()) {
    case Type::IDENTIFIER:
      if (IsNextTokenAssignment()) {
        return ParseAssignment();
      } else {
        std::unique_ptr<Expression> expression = ParseBinaryExpression();
        return std::move(expression);
      }
    case Type::IF:
      return ParseIf();
    case Type::FOR:
      return ParseFor();
    case Type::WHILE:
      return ParseWhile();
    case Type::BREAK:
      return ParseBreak();
    case Type::CONTINUE:
      return ParseContinue();
    case Type::FUNCTION:
      return ParseFunction();
    case Type::RETURN:
      return ParseReturn();
    case Type::EOL:
      Update();
      return ParseStatement();
    default:
      std::unique_ptr<Expression> expression = ParseBinaryExpression();
      return expression;
      break;
  }
}

bool Parser::IsNextTokenAssignment() {
  size_t previous_pos = current_;
  if (current_ + 1 >= tokens_.size()) {
    return false;
  }
  ++current_;
  bool res = false;
  Token next_token = tokens_[current_];
  if (Match(Type::ASSIGN, Type::PLUS_ASN, Type::MUNIS_ASN, Type::MUL_ASN,
            Type::DIV_ASN, Type::REM_ASN, Type::POW_ASN)) {
    res = true;
  }

  current_ = previous_pos;
  return res;
}

std::unique_ptr<IfNode> Parser::ParseIf() {
  Require(Type::IF);
  Update();
  std::unique_ptr<Expression> condition = ParseUntilToken(Type::THEN);
  Require(Type::THEN);
  std::unique_ptr<Statements> conditional_body =
      ParseUntilToken(Type::ELSE, Type::END);
  std::vector<
      std::pair<std::unique_ptr<Expression>, std::unique_ptr<Statements>>>
      else_if_block;
  size_t previous_pos = current_;

  while (MatchAll(Type::ELSE, Type::IF)) {
    previous_pos = current_;
    std::unique_ptr<Expression> new_condition = ParseUntilToken(Type::THEN);
    Require(Type::THEN);
    std::unique_ptr<Statements> new_conditional_body =
        ParseUntilToken(Type::ELSE, Type::END);
    else_if_block.emplace_back(std::move(new_condition),
                               std::move(new_conditional_body));
    current_ = previous_pos;
  }

  std::unique_ptr<Statements> else_block = nullptr;
  current_ = previous_pos;
  if (Match(Type::ELSE)) {
    else_block = ParseUntilToken(Type::END);
  }

  RequireAll(Type::END, Type::IF);

  return std::make_unique<IfNode>(
      std::move(condition), std::move(conditional_body),
      std::move(else_if_block), std::move(else_block));
}

std::unique_ptr<WhileNode> Parser::ParseWhile() {
  Require(Type::WHILE);
  std::unique_ptr<Expression> condition = ParseUntilToken(Type::EOL);
  Require(Type::EOL);
  std::unique_ptr<Statements> cycle_body = ParseUntilToken(Type::END);
  RequireAll(Type::END, Type::WHILE);

  return std::make_unique<WhileNode>(std::move(condition),
                                     std::move(cycle_body));
}

std::unique_ptr<ForNode> Parser::ParseFor() {
  Require(Type::FOR);
  size_t previous_pos = current_;
  Require(Type::IDENTIFIER);
  Token iteration = tokens_[previous_pos];
  Require(Type::IN);
  std::unique_ptr<Expression> condition = ParseUntilToken(Type::EOL);
  Require(Type::EOL);
  std::unique_ptr<Statements> cycle_body = ParseUntilToken(Type::END);
  RequireAll(Type::END, Type::FOR);

  return std::make_unique<ForNode>(iteration, std::move(condition),
                                   std::move(cycle_body));
}

std::unique_ptr<BreakNode> Parser::ParseBreak() {
  Require(Type::BREAK);
  return std::make_unique<BreakNode>();
}

std::unique_ptr<ContinueNode> Parser::ParseContinue() {
  Require(Type::CONTINUE);
  return std::make_unique<ContinueNode>();
}

template <typename... Types>
std::unique_ptr<Statements> Parser::ParseUntilToken(Types... types) {
  std::unique_ptr<Statements> block = std::make_unique<Statements>();

  while ((current_ < tokens_.size() - 1) && !Match(types...)) {
    block->AddNode(ParseStatement());
  }

  return block;
}

std::unique_ptr<FunctionNode> Parser::ParseFunction() {
  RequireAll(Type::LEFT_PAREN);
  std::vector<std::unique_ptr<Expression>> args;
  if (!Match(Type::RIGHT_PAREN)) {
    do {
      args.push_back(ParseBinaryExpression());
    } while (Match(Type::COMMA));
  }

  Require(Type::RIGHT_PAREN);

  std::unique_ptr<Statements> function_body = ParseUntilToken(Type::END);
  RequireAll(Type::END, Type::FUNCTION);

  return std::make_unique<FunctionNode>(std::move(args),
                                        std::move(function_body));
}

std::unique_ptr<ReturnNode> Parser::ParseReturn() {
  size_t line = tokens_[current_].GetLine();
  Require(Type::RETURN);
  std::unique_ptr<Expression> res;
  if ((current_ < tokens_.size() - 1)) {
    res = ParseBinaryExpression();
  }
  if (res == nullptr) {
    return std::make_unique<ReturnNode>();
  }
  return std::make_unique<ReturnNode>(std::move(res));
}

std::unique_ptr<Assignment> Parser::ParseAssignment() {
  size_t previous_pos = current_;
  Require(Type::IDENTIFIER);
  Token variable = tokens_[previous_pos];

  previous_pos = current_;
  Require(Type::ASSIGN, Type::PLUS_ASN, Type::MUNIS_ASN, Type::MUL_ASN,
          Type::DIV_ASN, Type::REM_ASN, Type::POW_ASN);
  Token operation = tokens_[previous_pos];

  std::unique_ptr<Expression> value = ParseBinaryExpression();

  return std::make_unique<Assignment>(operation, variable, std::move(value));
}

int Parser::GetPriority() {
  if (Match(Type::POW)) {
    return 1;
  }
  if (Match(Type::MUL, Type::DIV, Type::REM)) {
    return 2;
  }
  if (Match(Type::PLUS, Type::MINUS)) {
    return 3;
  }
  if (Match(Type::LESS, Type::LESS_EQUAL, Type::GREATER, Type::GREATER_EQUAL)) {
    return 4;
  }
  if (Match(Type::LESS, Type::LESS_EQUAL, Type::GREATER, Type::GREATER_EQUAL)) {
    return 5;
  }
  if (Match(Type::EQUAL, Type::NOT_EQUAL)) {
    return 6;
  }
  if (Match(Type::AND)) {
    return 7;
  }
  if (Match(Type::OR)) {
    return 8;
  } else {
    return 0;
  }
}

std::unique_ptr<Expression> Parser::ParseBinaryExpression(
    int highest_priority) {
  std::unique_ptr<Expression> left_part = ParseUnaryExpression();

  while (!Match(Type::EOL)) {
    Token token = tokens_[current_];
    int priority = GetPriority();

    if ((priority == 0) || (priority > highest_priority)) {
      break;
    }

    std::unique_ptr<Expression> right_part =
        ParseBinaryExpression(priority - 1);
    left_part = std::make_unique<BinaryExpression>(std::move(left_part), token,
                                                   std::move(right_part));
  }
  return left_part;
}

std::unique_ptr<Expression> Parser::ParseUnaryExpression() {
  Token token = tokens_[current_];
  if (token.GetType() == Type::MINUS || token.GetType() == Type::PLUS) {
    int sign = 1;
    while (true) {
      if (Match(Type::PLUS)) {
      } else if (Match(Type::MINUS)) {
        sign *= -1;
      } else {
        break;
      }
    }
    size_t previous = current_;
    std::unique_ptr<Expression> expr = ParseUnaryExpression();
    if (sign == -1) {
      return std::make_unique<UnaryExpression>(
          Token{Type::MINUS, std::string("-"), tokens_[previous].GetLine(),
                tokens_[previous].GetPos()},
          std::move(expr));
    } else {
      return std::make_unique<UnaryExpression>(
          Token{Type::PLUS, std::string("+"), tokens_[previous].GetLine(),
                tokens_[previous].GetPos()},
          std::move(expr));
    }
  }
  if (token.GetType() == Type::NOT) {
    int sign = -1;
    while (true) {
      if (Match(Type::NOT)) {
        sign *= -1;
      } else {
        break;
      }
    }
  }
  return ParsePrimary();
}

std::unique_ptr<Expression> Parser::ParsePrimary() {
  Token token = tokens_[current_];
  if (Match(Type::NIL)) {
    return std::make_unique<Nil>(token);
  }

  if (Match(Type::NUMBER)) {
    return std::make_unique<Number>(token);
  }

  if (Match(Type::BOOL)) {
    return std::make_unique<Bool>(token);
  }

  if (Match(Type::FUNCTION)) {
    return ParseFunction();
  }

  if (Match(Type::STRING)) {
    std::unique_ptr<StringLiteral> string =
        std::make_unique<StringLiteral>(token);
    if (current_ < tokens_.size() && Match(Type::LEFT_BRACKET)) {
      return ParseIndexOrSlice(std::move(string));
    }
    return std::move(string);
  }

  if (Match(Type::IDENTIFIER)) {
    if (current_ < tokens_.size() && Match(Type::LEFT_PAREN)) {
      Require(Type::LEFT_PAREN);
      std::vector<std::unique_ptr<Expression>> args;
      while (current_ < tokens_.size() && !Match(Type::RIGHT_PAREN)) {
        args.emplace_back(ParseUntilToken(Type::COMMA, Type::RIGHT_PAREN));
        --current_;
      }
      return std::make_unique<FunctionCall>(token.GetLexeme(), std::move(args));

    } else if (current_ < tokens_.size() && Match(Type::LEFT_BRACKET)) {
      std::unique_ptr<Variable> string = std::make_unique<Variable>(token);
      return ParseIndexOrSlice(std::move(string));

    } else {
      return std::make_unique<Variable>(token);
    }
  }

  if (Match(Type::LEFT_PAREN)) {
    auto expr = ParseBinaryExpression();
    Require(Type::RIGHT_BRACKET);
    return expr;
  }

  if (Match(Type::LEFT_BRACKET)) {
    --current_;
    if (!Match(Type::IDENTIFIER, Type::STRING)) {
      Require(Type::LEFT_BRACKET);
      std::vector<std::unique_ptr<Expression>> elements;
      while (current_ < tokens_.size() && !Match(Type::RIGHT_BRACKET)) {
        elements.emplace_back(
            ParseUntilToken(Type::COMMA, Type::RIGHT_BRACKET));
        --current_;
      }
      return std::make_unique<List>(std::move(elements));
    }
  }

  throw std::runtime_error(
      "Number, variable, string, bool or function call expected" +
      std::to_string(current_));
}

std::unique_ptr<Expression> Parser::ParseIndexOrSlice(
    std::unique_ptr<Expression> variable) {
  Require(Type::LEFT_BRACKET);
  std::unique_ptr<Expression> slice_start =
      std::make_unique<Nil>(Token(Type::NIL, "nil", 0, 0));
  if (Match(Type::COLON)) {
    Require(Type::COLON);
    std::unique_ptr<Expression> slice_end =
        std::make_unique<Nil>(Token(Type::NIL, "nil", 0, 0));
    if (!Match(Type::RIGHT_BRACKET)) {
      slice_end = ParseBinaryExpression();
    }
    Require(Type::RIGHT_BRACKET);
    return std::make_unique<Slice>(std::move(variable), std::move(slice_start),
                                   std::move(slice_end));
  } else {
    Update();
    slice_start = ParseBinaryExpression();
  }
  Require(Type::RIGHT_BRACKET);
  return std::make_unique<Index>(std::move(variable), std::move(slice_start));
}
