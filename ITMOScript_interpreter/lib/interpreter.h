#pragma once
#include <iostream>

#include "lexer.h"
#include "parser.h"
#include "standard_library.h"

bool interpret(std::istream& input, std::ostream& output);

class Interpreter {
 public:
  Interpreter(std::ostream& output, std::unique_ptr<Statements> ast_root, std::shared_ptr<Scope> global_scope)
      : output_(output), ast_root_(std::move(ast_root)), global_scope_(global_scope) {
  }

  void DoInterpretation();
  static std::vector<std::string> GetStack();

 private:
  Value UnfoldNode(Expression* node);
  Value UnfoldStatements(Statements* node);

  Value UnfoldIf(IfNode* node);
  Value UnfoldFor(ForNode* node);
  Value UnfoldWhile(WhileNode* node);

  Value UnfoldAssignment(Assignment* node);
  Value UnfoldBinaryExpression(BinaryExpression* node);
  Value UnfoldUnaryExpression(UnaryExpression* node);

  Value UnfoldList(List* node);
  Value UnfoldIndex(Index* node);
  Value UnfoldSlice(Slice* node);

  Value UnfoldFunction(FunctionNode* node);
  Value CallFunction(FunctionCall* node);

  Value EvaluateNumber(BinaryExpression* node, double left, double right);
  Value EvaluateStringWithString(BinaryExpression* node,
                                 const std::string& left,
                                 const std::string& right);
  Value EvaluateStringWithNumber(BinaryExpression* node,
                                 const std::string& left, double right);
  Value EvaluateListWithList(BinaryExpression* node,
                             const std::vector<Value>& left,
                             const std::vector<Value>& right);
  Value EvaluateListWithNumber(BinaryExpression* node,
                               const std::vector<Value>& left, double right);
  Value CallUserDeclaredFunction(std::shared_ptr<Function> func,
                                 const std::vector<Value>& args);

  std::ostream& output_;
  std::unique_ptr<Statements> ast_root_;
  std::shared_ptr<Scope> global_scope_;
  std::vector<std::string> stack_;
  static Interpreter* current_interpreter_;
};