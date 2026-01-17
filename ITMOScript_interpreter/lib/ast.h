#pragma once
#include <memory>
#include <string>
#include <vector>

#include "token.h"

struct Expression {
  Expression() = default;
  virtual ~Expression() = default;
};

class Statements : public Expression {
 public:
  Statements() = default;

  void AddNode(std::unique_ptr<Expression> node) {
    nodes_.emplace_back(std::move(node));
  }

  std::vector<std::unique_ptr<Expression>> GetNodes() { return std::move(nodes_); }

 private:
  std::vector<std::unique_ptr<Expression>> nodes_;
};

struct Variable : Expression {
  Token name;
  explicit Variable(const Token& name) : name(name) {}
};

struct Number : Expression {
  Token number;
  explicit Number(const Token& num) : number(num) {}
};

struct StringLiteral : Expression {
  Token value;
  explicit StringLiteral(const Token& val) : value(val) {}
};

struct Bool : Expression {
  Token value;
  explicit Bool(const Token& val) : value(val) {}
};

struct Nil : Expression {
  Token nil;
  explicit Nil(const Token& nul) : nil(nul) {}
};

struct BinaryExpression : Expression {
  std::unique_ptr<Expression> left;
  Token operation;
  std::unique_ptr<Expression> right;
  BinaryExpression(std::unique_ptr<Expression> left, const Token& oper,
                   std::unique_ptr<Expression> right)
      : left(std::move(left)), operation(oper), right(std::move(right)) {}
};

struct UnaryExpression : Expression {
  Token operation;
  std::unique_ptr<Expression> expression;
  UnaryExpression(const Token& oper, std::unique_ptr<Expression> expr)
      : operation(oper), expression(std::move(expr)) {}
};

struct IfNode : Expression {
  std::unique_ptr<Expression> condition;
  std::unique_ptr<Statements> cycle_body;
  std::vector<
      std::pair<std::unique_ptr<Expression>, std::unique_ptr<Statements>>>
      else_if_block;
  std::unique_ptr<Statements> else_block;

  IfNode(std::unique_ptr<Expression> cond, std::unique_ptr<Statements> actns,
         std::vector<std::pair<std::unique_ptr<Expression>,
                               std::unique_ptr<Statements>>>
             else_if,
         std::unique_ptr<Statements> else_)
      : condition(std::move(cond)),
        cycle_body(std::move(actns)),
        else_if_block(std::move(else_if)),
        else_block(std::move(else_)) {}
};

struct WhileNode : Expression {
  std::unique_ptr<Expression> condition;
  std::unique_ptr<Statements> cycle_body;

  WhileNode(std::unique_ptr<Expression> cond, std::unique_ptr<Statements> actns)
      : condition(std::move(cond)), cycle_body(std::move(actns)) {}
};

struct ForNode : Expression {
  Token iterator;
  std::unique_ptr<Expression> condition;
  std::unique_ptr<Statements> cycle_body;

  ForNode(const Token& iter, std::unique_ptr<Expression> cond,
          std::unique_ptr<Statements> actns)
      : iterator(std::move(iter)),
        condition(std::move(cond)),
        cycle_body(std::move(actns)) {}
};

struct BreakNode : public Expression {
  BreakNode() = default;
};

struct ContinueNode : public Expression {
  ContinueNode() = default;
};

struct ReturnNode : public Expression {
  ReturnNode(std::unique_ptr<Expression> value_) : value(std::move(value_)) {}
  ReturnNode() : value(nullptr) {};

  std::unique_ptr<Expression> value;
};

struct FunctionNode : Expression {
  std::vector<std::unique_ptr<Expression>> arguments;
  std::unique_ptr<Statements> function_body;
  FunctionNode(std::vector<std::unique_ptr<Expression>> args,
           std::unique_ptr<Statements> func_body)
      : function_body(std::move(func_body)), arguments(std::move(args)) {}
};

struct FunctionCall : public Expression {
  std::string function_name;
  std::vector<std::unique_ptr<Expression>> arguments;

  FunctionCall(const std::string& name,
               std::vector<std::unique_ptr<Expression>> args)
      : function_name(std::move(name)), arguments(std::move(args)) {}
};

struct Assignment : public Expression {
  Token variable;
  Token operation;
  std::unique_ptr<Expression> value;

  Assignment(const Token& var, const Token& oper,
             std::unique_ptr<Expression> val)
      : variable(std::move(var)),
        operation(std::move(oper)),
        value(std::move(val)) {}
};

struct List : public Expression {
  std::vector<std::unique_ptr<Expression>> list;

  List(std::vector<std::unique_ptr<Expression>> list) : list(std::move(list)) {}
};

struct Index : public Expression {
  std::unique_ptr<Expression> object;
  std::unique_ptr<Expression> index;

  Index(std::unique_ptr<Expression> obj, std::unique_ptr<Expression> ind)
      : object(std::move(obj)), index(std::move(ind)) {}
};

struct Slice : public Expression {
  std::unique_ptr<Expression> object;
  std::unique_ptr<Expression> start;
  std::unique_ptr<Expression> end;

  Slice(std::unique_ptr<Expression> obj, std::unique_ptr<Expression> srt,
        std::unique_ptr<Expression> end)
      : object(std::move(obj)), start(std::move(srt)), end(std::move(end)) {}
};