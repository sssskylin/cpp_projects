#include "standard_library.h"

void LoadStandardLibrary(std::shared_ptr<Scope> global_scope,
                         std::ostream& output) {
  LoadNumberFunctions(global_scope, output);
  LoadStringFunctions(global_scope, output);
  LoadListFunctions(global_scope, output);
  LoadSystemFunctions(global_scope, output);

  global_scope->Assign(
      "len", standard_func{[&output](const std::vector<Value>& args) -> Value {
        if (args.size() > 1) {
          throw std::runtime_error(
              "Too many arguments! Only one argument expected");
        }

        if (args.size() < 1) {
          throw std::runtime_error(
              "Not enough arguments! Only one argument expected");
        }

        double res;

        if (std::holds_alternative<std::string>(args[0])) {
          try {
            res = std::get<std::string>(args[0]).length();
          } catch (const std::exception& e) {
            throw std::runtime_error("Argument should be a string!");
          }
        }

        if (std::holds_alternative<std::vector<Value>>(args[0])) {
          try {
            res = std::get<std::vector<Value>>(args[0]).size();
          } catch (const std::exception& e) {
            throw std::runtime_error("Argument should be a list!");
          }
        }

        return Value(res);
      }});
}

void LoadNumberFunctions(std::shared_ptr<Scope> global_scope,
                         std::ostream& output) {
  global_scope->Assign(
      "abs", standard_func{[&output](const std::vector<Value>& args) -> Value {
        if (args.size() > 1) {
          throw std::runtime_error(
              "Too many arguments! Only one argument expected");
        }

        if (args.size() < 1) {
          throw std::runtime_error(
              "Not enough arguments! Only one argument expected");
        }

        double res;
        try {
          res = std::abs(std::get<double>(args[0]));
        } catch (const std::exception& e) {
          throw std::runtime_error("Argument should be a number!");
        }

        return Value(res);
      }});

  global_scope->Assign(
      "ceil", standard_func{[&output](const std::vector<Value>& args) -> Value {
        if (args.size() > 1) {
          throw std::runtime_error(
              "Too many arguments! Only one argument expected");
        }

        if (args.size() < 1) {
          throw std::runtime_error(
              "Not enough arguments! Only one argument expected");
        }

        double res;
        try {
          res = std::ceil(std::get<double>(args[0]));
        } catch (const std::exception& e) {
          throw std::runtime_error("Argument should be a number!");
        }

        return Value(res);
      }});

  global_scope->Assign(
      "floor",
      standard_func{[&output](const std::vector<Value>& args) -> Value {
        if (args.size() > 1) {
          throw std::runtime_error(
              "Too many arguments! Only one argument expected");
        }

        if (args.size() < 1) {
          throw std::runtime_error(
              "Not enough arguments! Only one argument expected");
        }

        double res;
        try {
          res = std::floor(std::get<double>(args[0]));
        } catch (const std::exception& e) {
          throw std::runtime_error("Argument should be a number!");
        }

        return Value(res);
      }});

  global_scope->Assign(
      "round",
      standard_func{[&output](const std::vector<Value>& args) -> Value {
        if (args.size() > 1) {
          throw std::runtime_error(
              "Too many arguments! Only one argument expected");
        }

        if (args.size() < 1) {
          throw std::runtime_error(
              "Not enough arguments! Only one argument expected");
        }

        double res;
        try {
          res = std::round(std::get<double>(args[0]));
        } catch (const std::exception& e) {
          throw std::runtime_error("Argument should be a number!");
        }

        return Value(res);
      }});

  global_scope->Assign(
      "sqrt", standard_func{[&output](const std::vector<Value>& args) -> Value {
        if (args.size() > 1) {
          throw std::runtime_error(
              "Too many arguments! Only one argument expected");
        }

        if (args.size() < 1) {
          throw std::runtime_error(
              "Not enough arguments! Only one argument expected");
        }

        double res;
        try {
          res = std::sqrt(std::get<double>(args[0]));
        } catch (const std::exception& e) {
          throw std::runtime_error("Argument should be a number!");
        }

        return Value(res);
      }});

  global_scope->Assign(
      "rnd", standard_func{[&output](const std::vector<Value>& args) -> Value {
        if (args.size() > 1) {
          throw std::runtime_error(
              "Too many arguments! Only one argument expected");
        }

        if (args.size() < 1) {
          throw std::runtime_error(
              "Not enough arguments! Only one argument expected");
        }

        double res;
        try {
          std::random_device rd;
          std::mt19937 gen(rd());
          std::uniform_real_distribution<> dist(0.0, std::get<double>(args[0]));
          res = dist(gen);
        } catch (const std::exception& e) {
          throw std::runtime_error("Argument should be a number!");
        }

        return Value(res);
      }});

  global_scope->Assign(
      "parse_num",
      standard_func{[&output](const std::vector<Value>& args) -> Value {
        if (args.size() > 1) {
          throw std::runtime_error(
              "Too many arguments! Only one argument expected");
        }

        if (args.size() < 1) {
          throw std::runtime_error(
              "Not enough arguments! Only one argument expected");
        }

        double res;
        try {
          res = std::stod(std::get<std::string>(args[0]));
        } catch (const std::exception& e) {
          return Value(nullptr);
        }

        return Value(res);
      }});

  global_scope->Assign(
      "to_string",
      standard_func{[&output](const std::vector<Value>& args) -> Value {
        if (args.size() > 1) {
          throw std::runtime_error(
              "Too many arguments! Only one argument expected");
        }

        if (args.size() < 1) {
          throw std::runtime_error(
              "Not enough arguments! Only one argument expected");
        }

        std::string res;
        try {
          res = std::to_string(std::get<double>(args[0]));
        } catch (const std::exception& e) {
          throw std::runtime_error("Argument should be a number!");
        }

        return Value(res);
      }});
}

void LoadStringFunctions(std::shared_ptr<Scope> global_scope,
                         std::ostream& output) {
  global_scope->Assign(
      "lower",
      standard_func{[&output](const std::vector<Value>& args) -> Value {
        if (args.size() > 1) {
          throw std::runtime_error(
              "Too many arguments! Only one argument expected");
        }

        if (args.size() < 1) {
          throw std::runtime_error(
              "Not enough arguments! Only one argument expected");
        }

        std::string res;
        try {
          std::string str = std::get<std::string>(args[0]);
          std::transform(str.begin(), str.end(), str.begin(),
                         [](char ch) { return std::tolower(ch); });
        } catch (const std::exception& e) {
          throw std::runtime_error("Argument should be a string!");
        }

        return Value(res);
      }});

  global_scope->Assign(
      "upper",
      standard_func{[&output](const std::vector<Value>& args) -> Value {
        if (args.size() > 1) {
          throw std::runtime_error(
              "Too many arguments! Only one argument expected");
        }

        if (args.size() < 1) {
          throw std::runtime_error(
              "Not enough arguments! Only one argument expected");
        }

        std::string res;
        try {
          std::string str = std::get<std::string>(args[0]);
          std::transform(str.begin(), str.end(), str.begin(),
                         [](char ch) { return std::toupper(ch); });
        } catch (const std::exception& e) {
          throw std::runtime_error("Argument should be a string!");
        }

        return Value(res);
      }});

  global_scope->Assign(
      "split",
      standard_func{[&output](const std::vector<Value>& args) -> Value {
        if (args.size() > 2) {
          throw std::runtime_error(
              "Too many arguments! Only two argument expected");
        }

        if (args.size() < 2) {
          throw std::runtime_error(
              "Not enough arguments! Only two argument expected");
        }

        std::string res, delim;
        std::vector<Value> parts;
        try {
          res = std::get<std::string>(args[0]);
          delim = std::get<std::string>(args[1]);

          size_t pos = 0, next;
          while ((next = res.find(delim, pos)) != std::string::npos) {
            parts.emplace_back(res.substr(pos, next - pos));
            pos = next + delim.size();
          }

          parts.emplace_back(res.substr(pos));
        } catch (const std::exception& e) {
          throw std::runtime_error("Argument should be a string!");
        }

        return Value(res);
      }});

  global_scope->Assign(
      "join", standard_func{[&output](const std::vector<Value>& args) -> Value {
        if (args.size() > 2) {
          throw std::runtime_error(
              "Too many arguments! Only two argument expected");
        }

        if (args.size() < 2) {
          throw std::runtime_error(
              "Not enough arguments! Only two argument expected");
        }

        std::string res, delim;
        try {
          std::vector<Value> list = std::get<std::vector<Value>>(args[0]);
          delim = std::get<std::string>(args[1]);
          res = std::get<std::string>(list[0]);
          for (int i = 1; i < list.size(); ++i) {
            std::string elem = std::get<std::string>(list[i]);
            res += delim;
            res += elem;
          }
        } catch (const std::exception& e) {
          throw std::runtime_error("Argument should be a string!");
        }

        return Value(res);
      }});
  global_scope->Assign(
      "replace",
      standard_func{[&output](const std::vector<Value>& args) -> Value {
        if (args.size() > 3) {
          throw std::runtime_error(
              "Too many arguments! Only three argument expected");
        }

        if (args.size() < 3) {
          throw std::runtime_error(
              "Not enough arguments! Only three argument expected");
        }

        std::string res, old_part, new_part;
        try {
          res = std::get<std::string>(args[0]);
          old_part = std::get<std::string>(args[1]);
          new_part = std::get<std::string>(args[2]);
          size_t pos = 0;

          while ((pos = res.find(old_part, pos)) != std::string::npos) {
            res.replace(pos, old_part.length(), new_part);
            pos += new_part.length();
          }
        } catch (const std::exception& e) {
          throw std::runtime_error("Argument should be a string!");
        }

        return Value(res);
      }});
}

void LoadListFunctions(std::shared_ptr<Scope> global_scope,
                       std::ostream& output) {
  global_scope->Assign(
      "range",
      standard_func{[&output](const std::vector<Value>& args) -> Value {
        if (args.size() > 3) {
          throw std::runtime_error(
              "Too many arguments! Only three argument expected");
        }

        int start, end, step;
        std::vector<Value> res;
        try {
          if (args.size() == 1) {
            start = 0;
            end = static_cast<int>(std::get<double>(args[0]));
            step = 1;
          }
          if (args.size() == 2) {
            start = static_cast<int>(std::get<double>(args[0]));
            end = static_cast<int>(std::get<double>(args[1]));
            step = 1;
          }
          if (args.size() == 3) {
            start = static_cast<int>(std::get<double>(args[0]));
            end = static_cast<int>(std::get<double>(args[1]));
            step = static_cast<int>(std::get<double>(args[2]));
          }

          if (step == 0) {
            throw std::runtime_error(
                "Step is zero, which creates endless range");
          }

          for (double i = start; i < end; i += step) {
            res.push_back(i);
          }

        } catch (const std::exception& e) {
          throw std::runtime_error("Argument should be a number!");
        }

        return Value(res);
      }});

  global_scope->Assign(
      "push", standard_func{[&output](const std::vector<Value>& args) -> Value {
        if (args.size() > 2) {
          throw std::runtime_error(
              "Too many arguments! Only two argument expected");
        }

        if (args.size() < 2) {
          throw std::runtime_error(
              "Not enough arguments! Only two argument expected");
        }

        std::vector<Value> list;
        try {
          std::vector<Value> list = std::get<std::vector<Value>>(args[0]);
          list.emplace_back(args[1]);
        } catch (const std::exception& e) {
          throw std::runtime_error("Argument should be a list!");
        }

        return Value(list);
      }});

  global_scope->Assign(
      "pop", standard_func{[&output](const std::vector<Value>& args) -> Value {
        if (args.size() > 1) {
          throw std::runtime_error(
              "Too many arguments! Only one argument expected");
        }

        if (args.size() < 1) {
          throw std::runtime_error(
              "Not enough arguments! Only one argument expected");
        }

        std::vector<Value> list;
        Value res;
        try {
          std::vector<Value> list = std::get<std::vector<Value>>(args[0]);
          res = list[list.size() - 1];
          list.pop_back();
        } catch (const std::exception& e) {
          throw std::runtime_error("Argument should be a list!");
        }

        return Value(res);
      }});

  global_scope->Assign(
      "insert",
      standard_func{[&output](const std::vector<Value>& args) -> Value {
        if (args.size() > 3) {
          throw std::runtime_error(
              "Too many arguments! Only three argument expected");
        }

        if (args.size() < 3) {
          throw std::runtime_error(
              "Not enough arguments! Only three argument expected");
        }

        std::vector<Value> list;
        try {
          std::vector<Value> list = std::get<std::vector<Value>>(args[0]);
          double index = std::get<double>(args[1]);
          list.insert(list.begin() + index, args[2]);
        } catch (const std::exception& e) {
          throw std::runtime_error("Wrong argument type!");
        }

        return Value(list);
      }});

  global_scope->Assign(
      "remove",
      standard_func{[&output](const std::vector<Value>& args) -> Value {
        if (args.size() > 2) {
          throw std::runtime_error(
              "Too many arguments! Only two argument expected");
        }

        if (args.size() < 2) {
          throw std::runtime_error(
              "Not enough arguments! Only two argument expected");
        }

        std::vector<Value> list;
        Value res;
        try {
          std::vector<Value> list = std::get<std::vector<Value>>(args[0]);
          double index = std::get<double>(args[1]);
          res = list[index];
          list.erase(list.begin() + index);
        } catch (const std::exception& e) {
          throw std::runtime_error("Argument should be a list!");
        }

        return Value(res);
      }});
  global_scope->Assign(
      "sort", standard_func{[&output](const std::vector<Value>& args) -> Value {
        if (args.size() > 1) {
          throw std::runtime_error(
              "Too many arguments! Only one argument expected");
        }

        if (args.size() < 1) {
          throw std::runtime_error(
              "Not enough arguments! Only one argument expected");
        }

        std::vector<Value> list = std::get<std::vector<Value>>(args[0]);
        try {
          auto ref = list[0].index();
          for (auto& elem : list) {
            if (elem.index() != ref) {
              return Value(list);
            }
          }

          auto comparator = [&](const Value& a, const Value& b) {
            if (std::holds_alternative<double>(list[0])) {
              return std::get<double>(a) < std::get<double>(b);
            } else if (std::holds_alternative<std::string>(list[0])) {
              return std::get<std::string>(a) < std::get<std::string>(b);
            } else if (std::holds_alternative<bool>(list[0])) {
              return std::get<bool>(a) < std::get<bool>(b);
            } else {
              throw std::runtime_error("Unsorted type");
            }
          };

          std::sort(list.begin(), list.end(), comparator);
          return Value(std::move(list));
        } catch (const std::exception& e) {
          throw std::runtime_error("Argument should be a list!");
        }
      }});
}

void LoadSystemFunctions(std::shared_ptr<Scope> global_scope,
                         std::ostream& output) {
  global_scope->Assign(
      "print",
      standard_func{[&output](const std::vector<Value>& args) -> Value {
        bool is_ln = 0;
        Print(args, output, is_ln);
        return nullptr;
      }});
  global_scope->Assign(
      "println",
      standard_func{[&output](const std::vector<Value>& args) -> Value {
        bool is_ln = 1;
        Print(args, output, is_ln);
        return nullptr;
      }});
  global_scope->Assign("read", standard_func{[](auto& args) -> Value {
                         std::string line;
                         if (!std::getline(std::cin, line)) {
                           return nullptr;
                         }
                         return Value(line);
                       }});
  global_scope->Assign(
      "stacktrace",
      standard_func{[&output](const std::vector<Value>& args) -> Value {
        std::vector<Value> out;
        for (const auto& frame : Interpreter::GetStack()) {
          out.emplace_back(frame);
        }
        return Value::MakeList(std::move(out));
      }});
}

void Print(const std::vector<Value>& args, std::ostream& output, bool is_ln) {
  for (int i = 0; i < args.size(); ++i) {
    if (i > 0) {
      std::cout << " ";
    }
    if (auto out = std::get_if<double>(&args[i])) {
      std::cout << *out;
      if (is_ln) {
        std::cout << "\n";
      }
    } else if (auto out = std::get_if<std::string>(&args[i])) {
      std::cout << *out;
      if (is_ln) {
        std::cout << "\n";
      }
    } else if (auto out = std::get_if<bool>(&args[i])) {
      std::string res = "true";
      if (*out == 0) {
        res = "false";
      }
      std::cout << res;
      if (is_ln) {
        std::cout << "\n";
      }
    } else if (auto out = std::get_if<std::nullptr_t>(&args[i])) {
      std::cout << "nil";
      if (is_ln) {
        std::cout << "\n";
      }
    } else if (auto out = std::get_if<std::vector<Value>>(&args[i])) {
      std::cout << "[";
      Print(*out, output, is_ln);
      std::cout << "]";
      if (is_ln) {
        std::cout << "\n";
      }
    }
  }
}
