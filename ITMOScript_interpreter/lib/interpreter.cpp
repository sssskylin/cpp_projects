#include "interpreter.h"

#include <cmath>

Interpreter* Interpreter::current_interpreter_ = nullptr;

bool interpret(std::istream& input, std::ostream& output) {
  try {
    Lexer lexer(input);
    lexer.Tokenize();
    Parser parser(lexer.GetTokens());
    auto global_scope = std::make_shared<Scope>();
    LoadStandardLibrary(global_scope, output);

    Interpreter interpreter(output, parser.ParseCode(), global_scope);
    interpreter.DoInterpretation();
  } catch (const std::exception& ex) {
    output << ex.what() << "\n";
    return false;
  }
  return true;
}

void Interpreter::DoInterpretation() {
  std::vector<std::unique_ptr<Expression>> scopes_vector =
      ast_root_->GetNodes();
  for (auto& node : scopes_vector) {
    UnfoldNode(node.get());
  }
}

Value Interpreter::UnfoldNode(Expression* node) {
  if (auto node_type = dynamic_cast<Variable*>(node)) {
    return Value(global_scope_->FindVariable(node_type->name.GetLexeme()));
  }
  if (auto node_type = dynamic_cast<Number*>(node)) {
    return Value(std::stod(node_type->number.GetLexeme()));
  }
  if (auto node_type = dynamic_cast<StringLiteral*>(node)) {
    return Value(node_type->value.GetLexeme());
  }
  if (auto node_type = dynamic_cast<Bool*>(node)) {
    if (node_type->value.GetLexeme() == "true") {
      return Value(true);
    } else {
      return Value(false);
    }
  }
  if (auto node_type = dynamic_cast<Nil*>(node)) {
    return Value(nullptr);
  }
  if (auto node_type = dynamic_cast<BinaryExpression*>(node)) {
    return UnfoldBinaryExpression(node_type);
  }
  if (auto node_type = dynamic_cast<UnaryExpression*>(node)) {
    return UnfoldUnaryExpression(node_type);
  }
  if (auto node_type = dynamic_cast<IfNode*>(node)) {
    return UnfoldIf(node_type);
  }
  if (auto node_type = dynamic_cast<WhileNode*>(node)) {
    return UnfoldWhile(node_type);
  }
  if (auto node_type = dynamic_cast<ForNode*>(node)) {
    return UnfoldFor(node_type);
  }
  if (auto node_type = dynamic_cast<FunctionNode*>(node)) {
    return UnfoldFunction(node_type);
  }
  if (auto node_type = dynamic_cast<BreakNode*>(node)) {
    throw Break{};
  }
  if (auto node_type = dynamic_cast<ContinueNode*>(node)) {
    throw Continue{};
  }
  if (auto node_type = dynamic_cast<ReturnNode*>(node)) {
    Value value = UnfoldNode(node_type->value.get());
    throw Return(value);
  }
  if (auto node_type = dynamic_cast<FunctionCall*>(node)) {
    return CallFunction(node_type);
  }
  if (auto node_type = dynamic_cast<Assignment*>(node)) {
    return UnfoldAssignment(node_type);
  }
  if (auto node_type = dynamic_cast<List*>(node)) {
    return UnfoldList(node_type);
  }
  if (auto node_type = dynamic_cast<Index*>(node)) {
    return UnfoldIndex(node_type);
  }
  if (auto node_type = dynamic_cast<Slice*>(node)) {
    return UnfoldSlice(node_type);
  }
  if (auto node_type = dynamic_cast<Statements*>(node)) {
    return UnfoldStatements(node_type);
  }
  throw std::runtime_error("Unknown node!");
}

Value Interpreter::UnfoldAssignment(Assignment* node) {
  if (node->operation.GetType() == Type::ASSIGN) {
    Value value = UnfoldNode(node->value.get());
    if (auto func_value = std::get_if<std::shared_ptr<Function>>(&value)) {
      (*func_value)->name = node->variable.GetLexeme();
    }
    global_scope_->Assign(node->variable.GetLexeme(), value);
    return value;
  }
  double variable_value =
      std::get<double>(global_scope_->FindVariable(node->variable.GetLexeme()));
  double value = std::get<double>(UnfoldNode(node->value.get()));
  switch (node->operation.GetType()) {
    case Type::DIV_ASN:
      global_scope_->Assign(node->variable.GetLexeme(), variable_value / value);
      return value;
    case Type::MUL_ASN:
      global_scope_->Assign(node->variable.GetLexeme(), variable_value * value);
      return value;
    case Type::POW_ASN:
      global_scope_->Assign(node->variable.GetLexeme(),
                            pow(variable_value, value));
      return value;
    case Type::REM_ASN:
      global_scope_->Assign(node->variable.GetLexeme(),
                            std::fmod(variable_value, value));
      return value;
    case Type::PLUS_ASN:
      global_scope_->Assign(node->variable.GetLexeme(), variable_value + value);
      return value;
    case Type::MUNIS_ASN:
      global_scope_->Assign(node->variable.GetLexeme(), variable_value - value);
      return value;
  }
  return nullptr;
}

Value Interpreter::UnfoldBinaryExpression(BinaryExpression* node) {
  auto left_value = UnfoldNode(node->left.get());
  auto right_value = UnfoldNode(node->right.get());

  if (auto left = std::get_if<double>(&left_value)) {
    if (auto right = std::get_if<double>(&right_value)) {
      return EvaluateNumber(node, *left, *right);
    }
    if (auto right = std::get_if<std::string>(&right_value)) {
      return EvaluateStringWithNumber(node, *right, *left);
    }
    if (auto right = std::get_if<std::vector<Value>>(&right_value)) {
      return EvaluateListWithNumber(node, *right, *left);
    }
  }

  if (auto left = std::get_if<std::string>(&left_value)) {
    if (auto right = std::get_if<double>(&right_value)) {
      return EvaluateStringWithNumber(node, *left, *right);
    }
    if (auto right = std::get_if<bool>(&right_value)) {
      return EvaluateStringWithNumber(node, *left, *right);
    }
    if (auto right = std::get_if<std::string>(&right_value)) {
      return EvaluateStringWithString(node, *left, *right);
    }
  }

  if (auto left = std::get_if<std::vector<Value>>(&left_value)) {
    if (auto right = std::get_if<std::vector<Value>>(&right_value)) {
      return EvaluateListWithList(node, *left, *right);
    }
    if (auto right = std::get_if<double>(&right_value)) {
      return EvaluateListWithNumber(node, *left, *right);
    }
  }

  throw std::runtime_error("Unknown operation");
}

Value Interpreter::UnfoldStatements(Statements* node) {
  Value value = nullptr;
  std::vector<std::unique_ptr<Expression>> scopes_vector =
      ast_root_->GetNodes();
  for (auto& inner_node : scopes_vector) {
    value = UnfoldNode(inner_node.get());
  }
  return value;
}

Value Interpreter::EvaluateStringWithString(BinaryExpression* node,
                                            const std::string& left,
                                            const std::string& right) {
  switch (node->operation.GetType()) {
    case Type::PLUS:
      return left + right;
    case Type::MINUS:
      if ((left.length() >= right.length()) &&
          (left.substr(left.length() - right.length(), right.length()) ==
           right)) {
        return left.substr(0, left.length() - right.length());
      } else {
        throw std::runtime_error("Right isn't left suffix");
      }
    case Type::EQUAL:
      return (left == right);
    case Type::NOT_EQUAL:
      return (left != right);
    case Type::GREATER:
      return (left > right);
    case Type::LESS:
      return (left < right);
    case Type::GREATER_EQUAL:
      return (left >= right);
    case Type::LESS_EQUAL:
      return (left <= right);
    default:
      throw std::runtime_error("Unknown operator");
  }
}

Value Interpreter::EvaluateNumber(BinaryExpression* node, double left,
                                  double right) {
  switch (node->operation.GetType()) {
    case Type::PLUS:
      return left + right;
    case Type::MINUS:
      return left - right;
    case Type::MUL:
      return left * right;
    case Type::DIV:
      if (right == 0.0) {
        throw std::runtime_error("It's impossible to divide by zero");
      }
      return left / right;
    case Type::REM:
      if (right == 0.0) {
        throw std::runtime_error("It's impossible to divide by zero");
      }
      return std::fmod(left, right);
    case Type::POW:
      return pow(left, right);
    case Type::EQUAL:
      return (left == right);
    case Type::NOT_EQUAL:
      return (left != right);
    case Type::GREATER:
      return (left > right);
    case Type::LESS:
      return (left < right);
    case Type::GREATER_EQUAL:
      return (left >= right);
    case Type::LESS_EQUAL:
      return (left <= right);
    default:
      throw std::runtime_error("Unknown operator");
  }
}

Value Interpreter::EvaluateStringWithNumber(BinaryExpression* node,
                                            const std::string& left,
                                            double right) {
  std::string res = "";
  int integer = 0;
  double frac = 0.0;
  switch (node->operation.GetType()) {
    case Type::MUL:
      if (right < 0) {
        throw std::runtime_error(
            "It's impossible to multiply string by negative number");
      }
      integer = static_cast<int>(right);
      frac = right - integer;
      for (int i = 0; i < integer; ++i) {
        res += left;
      }
      for (int i = 0; i < frac * left.length(); ++i) {
        res += left[i];
      }
      return res;
    default:
      throw std::runtime_error("Unknown operator");
  }
}

Value Interpreter::EvaluateListWithNumber(BinaryExpression* node,
                                          const std::vector<Value>& left,
                                          double right) {
  std::vector<Value> res;
  int integer = 0;
  double frac = 0.0;
  switch (node->operation.GetType()) {
    case Type::MUL:
      if (right < 0) {
        throw std::runtime_error(
            "It's impossible to multiply string by negative number");
      }
      integer = static_cast<int>(right);
      frac = right - integer;
      for (int i = 0; i < integer; ++i) {
        for (int j = 0; j < left.size(); ++j) {
          res.emplace_back(left[j]);
        }
      }
      for (int i = 0; i < frac * left.size(); ++i) {
        res.emplace_back(left[i]);
      }
      return res;
    default:
      throw std::runtime_error("Unknown operator");
  }
}

Value Interpreter::EvaluateListWithList(BinaryExpression* node,
                                        const std::vector<Value>& left,
                                        const std::vector<Value>& right) {
  std::vector<Value> res;
  switch (node->operation.GetType()) {
    case Type::PLUS:
      for (int i = 0; i < left.size(); ++i) {
        res.push_back(left[i]);
      }
      for (int i = 0; i < right.size(); ++i) {
        res.push_back(right[i]);
      }
      return Value(res);
    default:
      throw std::runtime_error("Unknown operator");
  }
}

Value Interpreter::UnfoldUnaryExpression(UnaryExpression* node) {
  auto value = UnfoldNode(node->expression.get());
  switch (node->operation.GetType()) {
    case Type::MINUS:
      if (!std::holds_alternative<double>(value)) {
        throw std::runtime_error("Unary operations are only for numbers");
      }
      return -std::get<double>(value);
    case Type::PLUS:
      if (!std::holds_alternative<double>(value)) {
        throw std::runtime_error("Unary operations are only for numbers");
      }
      return std::get<double>(value);
  }
}

Value Interpreter::UnfoldIf(IfNode* node) {
  Value condition = UnfoldNode(node->condition.get());
  if (std::get<bool>(condition)) {
    return UnfoldStatements(node->cycle_body.get());
  }
  for (auto& [cond, block] : node->else_if_block) {
    Value another_condition = UnfoldNode(cond.get());
    if (std::get<bool>(another_condition)) {
      return UnfoldStatements(block.get());
    }
  }

  if (node->else_block) {
    return UnfoldStatements(node->else_block.get());
  }
  return nullptr;
}

Value Interpreter::UnfoldFor(ForNode* node) {
  Value iterable = UnfoldNode(node->condition.get());

  if (!std::holds_alternative<std::vector<Value>>(iterable)) {
    throw std::runtime_error("Wrong for loop declaration");
  }

  const std::string& it = node->iterator.GetLexeme();
  const auto& list = std::get<std::vector<Value>>(iterable);

  for (const Value& item : list) {
    global_scope_->Assign(it, item);
    UnfoldNode(node->cycle_body.get());
  }

  return Value();
}

Value Interpreter::UnfoldWhile(WhileNode* node) {
  Value value = nullptr;
  while (std::get<bool>(UnfoldNode(node->condition.get()))) {
    try {
      value = UnfoldStatements(node->cycle_body.get());
    } catch (Continue) {
      continue;
    } catch (Break) {
      break;
    }
  }
  return value;
}

Value Interpreter::UnfoldList(List* node) {
  std::vector<Value> res;
  for (const auto& elem : node->list) {
    res.push_back(UnfoldNode(elem.get()));
  }
  return Value::MakeList(std::move(res));
}

Value Interpreter::UnfoldIndex(Index* node) {
  auto ind = UnfoldNode(node->index.get());
  auto object = UnfoldNode(node->object.get());
  if (!std::holds_alternative<double>(ind)) {
    throw std::runtime_error("Index must be a number!");
  }

  int index = static_cast<int>(std::get<double>(ind));

  if (std::get<double>(ind) != index) {
    throw std::runtime_error("Index must be an integer!");
  }

  if (auto str = std::get_if<std::string>(&object)) {
    if (index < 0 || index >= static_cast<int>(str->length())) {
      throw std::runtime_error("Index is out of range");
    }

    return Value(std::string(1, (*str)[index]));
  }

  if (auto list = std::get_if<std::vector<Value>>(&object)) {
    if (index < 0 || index >= static_cast<int>(list->size())) {
      throw std::runtime_error("Index is out of range");
    }

    return list[index];
  }

  return nullptr;
}

Value Interpreter::UnfoldSlice(Slice* node) {
  auto start_ind = UnfoldNode(node->start.get());
  auto end_ind = UnfoldNode(node->end.get());
  auto object = UnfoldNode(node->object.get());
  if (!std::holds_alternative<double>(start_ind)) {
    throw std::runtime_error("Start index must be a number!");
  }
  if (!std::holds_alternative<double>(end_ind)) {
    throw std::runtime_error("End ndex must be a number!");
  }

  int start_index = static_cast<int>(std::get<double>(start_ind));
  int end_index = static_cast<int>(std::get<double>(end_ind));

  if (std::get<double>(start_ind) != start_index) {
    throw std::runtime_error("Start index must be an integer!");
  }

  if (std::get<double>(end_ind) != end_index) {
    throw std::runtime_error("End index must be an integer!");
  }

  if (auto str = std::get_if<std::string>(&object)) {
    if (start_index < 0 || start_index >= static_cast<int>(str->length()) ||
        end_index < 0 || end_index >= static_cast<int>(str->length())) {
      throw std::runtime_error("Index is out of range");
    }
    return Value(str->substr(start_index, end_index - start_index + 1));
  }

  if (auto list = std::get_if<std::vector<Value>>(&object)) {
    if (start_index < 0 || start_index >= static_cast<int>(list->size()) ||
        end_index < 0 || end_index >= static_cast<int>(list->size())) {
      throw std::runtime_error("Index is out of range");
    }

    std::vector<Value> res_list;
    for (int i = start_index; i < end_index; ++i) {
      res_list.emplace_back(list[i]);
    }

    return Value(res_list);
  }

  return nullptr;
}

Value Interpreter::UnfoldFunction(FunctionNode* node) {
  Function function;
  function.closure = global_scope_;
  function.args.reserve(node->arguments.size());
  for (const auto& arg : node->arguments) {
    if (auto varibl = dynamic_cast<Variable*>(arg.get())) {
      function.args.push_back(varibl->name.GetLexeme());
    } else {
      throw std::runtime_error("It should be a first-class function");
    }
  }

  function.body =
      std::make_unique<Statements>(std::move(*(node->function_body)));

  return Value(std::make_shared<Function>(std::move(function)));
}

Value Interpreter::CallFunction(FunctionCall* node) {
  stack_.emplace_back(node->function_name);
  auto function_value = global_scope_->FindVariable(node->function_name);
  std::vector<Value> args;
  for (const auto& elem : node->arguments) {
    args.emplace_back(UnfoldNode(elem.get()));
  }
  if (auto func = std::get_if<standard_func>(&function_value)) {
    stack_.pop_back();
    return (*func)(args);
  }
  if (auto func = std::get_if<std::shared_ptr<Function>>(&function_value)) {
    auto res = CallUserDeclaredFunction(*func, args);
    stack_.pop_back();
    return res;
  }
  throw std::runtime_error("Object " + node->function_name +
                           "is not a function");
}

Value Interpreter::CallUserDeclaredFunction(std::shared_ptr<Function> func,
                                            const std::vector<Value>& args) {
  if (args.size() != func->args.size()) {
    throw std::runtime_error("Invalid number of args");
  }

  auto local_scope = std::make_shared<Scope>(func->closure);
  for (size_t i = 0; i < args.size(); ++i) {
    local_scope->Assign(func->args[i], args[i]);
  }

  auto global_scope = global_scope_;
  global_scope_ = local_scope;

  Value res;
  try {
    res = UnfoldNode(func->body.get());
  } catch (const Return& rtrn) {
    res = rtrn.value;
  }

  global_scope_ = global_scope;
  return res;
}

std::vector<std::string> Interpreter::GetStack() {
  if (current_interpreter_ == nullptr) {
    return {};
  }
  return current_interpreter_->stack_;
}