#pragma once
#include <functional>
#include <variant>

#include "lexer.h"
#include "parser.h"

class Value;

using standard_func = std::function<Value(const std::vector<Value>&)>;

struct Scope;

struct Function {
  std::string name;
  std::vector<std::string> args;
  std::unique_ptr<Statements> body;
  std::shared_ptr<Scope> closure;
};

class Value : public std::variant<double, std::string, bool, std::nullptr_t,
                                  standard_func, std::vector<Value>,
                                  std::shared_ptr<Function> > {
 public:
  using base =
      std::variant<double, std::string, bool, std::nullptr_t, standard_func,
                   std::vector<Value>, std::shared_ptr<Function>>;
  using base::base;
  using base::operator=;

  static Value MakeList(std::vector<Value>&& elems) {
    return Value(std::move(elems));
  }
};

class Scope {
 public:
  Scope() = default;
  Scope(std::shared_ptr<Scope> parenting_scope)
      : parenting_scope_(std::move(parenting_scope)) {}

  Value FindVariable(const std::string& variable);
  void Assign(const std::string& variable, const Value& value);

 private:
  std::map<std::string, Value> variables_;
  std::shared_ptr<Scope> parenting_scope_;
};

struct Break : std::exception {};
struct Continue : std::exception {};
struct Return : std::exception {
  Value value;
  Return(Value val) : value(val) {}
};