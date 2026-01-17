#pragma once
#include <iostream>
#include <stdexcept>
#include <vector>

#include "ast.h"
#include "token.h"

class Parser {
 public:
  Parser(std::vector<Token> tokens) : tokens_(std::move(tokens)), current_(0) {}
  std::unique_ptr<Statements> ParseCode();

 private:
  std::vector<Token> tokens_;
  size_t current_;

  template <typename... Types>
  bool Match(Types... types);
  template <typename... Types>
  bool MatchAll(Types... types);
  bool Check(Type type) const;
  const Token& Update();
  template <typename... Types>
  void Require(Types... types);
  template <typename... Types>
  void RequireAll(Types... types);

  std::unique_ptr<Expression> ParseStatement();
  std::unique_ptr<IfNode> ParseIf();
  std::unique_ptr<ForNode> ParseFor();
  std::unique_ptr<WhileNode> ParseWhile();
  std::unique_ptr<BreakNode> ParseBreak();
  std::unique_ptr<ContinueNode> ParseContinue();
  std::unique_ptr<FunctionNode> ParseFunction();
  std::unique_ptr<ReturnNode> ParseReturn();
  std::unique_ptr<Assignment> ParseAssignment();
  template <typename... Types>
  std::unique_ptr<Statements> ParseUntilToken(Types... types);
  std::unique_ptr<Expression> ParseBinaryExpression(int highest_priority = 8);
  std::unique_ptr<Expression> ParseUnaryExpression();
  std::unique_ptr<Expression> ParsePrimary();
  std::unique_ptr<Expression> ParseIndexOrSlice(
      std::unique_ptr<Expression> variable);

  bool IsNextTokenAssignment();
  int GetPriority();
};
