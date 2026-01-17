#include "scope_value.h"

Value Scope::FindVariable(const std::string& variable) {
  if (variables_.count(variable)) {
    return variables_[variable];
  }
  if (parenting_scope_) {
    return parenting_scope_->FindVariable(variable);
  }
  throw std::runtime_error("Use of undeclared identifier!");
}

void Scope::Assign(const std::string& variable, const Value& value) {
  if (variables_.count(variable) || !parenting_scope_) {
    variables_[variable] = value;
  } else {
    parenting_scope_->Assign(variable, value);
  }
}