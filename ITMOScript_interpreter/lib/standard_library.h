#pragma once
#include <random>

#include "scope_value.h"
#include "interpreter.h"

class Value;

using standard_func = std::function<Value(const std::vector<Value>&)>;

struct Scope;

class Interpreter;

void LoadStandardLibrary(std::shared_ptr<Scope> global_scope,
                         std::ostream& output);

void LoadNumberFunctions(std::shared_ptr<Scope> global_scope,
                         std::ostream& output);
void LoadStringFunctions(std::shared_ptr<Scope> global_scope,
                         std::ostream& output);
void LoadListFunctions(std::shared_ptr<Scope> global_scope,
                       std::ostream& output);
void LoadSystemFunctions(std::shared_ptr<Scope> global_scope,
                         std::ostream& output);

void Print(const std::vector<Value>& args, std::ostream& output, bool is_ln);