#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <iostream>

#pragma once

namespace ArgumentParser {

template <typename Type>

class Argument {
 public:
  std::string short_arg;
  std::string arg;
  std::string description;

  bool is_multi_value = false;
  Type value;
  std::vector<Type> values;
  int min_args_count = 0;

  bool has_default = false;
  Type default_value;

  bool has_storage = false;
  Type* storage;
  std::vector<Type>* multi_storage;

  bool is_positional = false;

  Argument() = default;

  Argument(std::string arg) {
    this->arg = arg;
  }

  Argument(std::string short_arg, std::string arg, bool command) {
    if (command) {
      this->arg = short_arg;
      this->description = arg;
    } else {
      this->short_arg = short_arg;
      this->arg = arg;
    }
  }

  Argument(std::string short_arg, std::string arg, std::string description) {
    this->short_arg = short_arg;
    this->arg = arg;
    this->description = description;
  }

  Argument& Default(Type value) {
    this->value = value;
    this->has_default = true;
    return *this;
  }

  Argument& Default(std::vector<Type> values) {
    values = values;
    has_default = true; 
    return *this;
  }

  Argument& StoreValue(Type& value) {
    this->has_storage = true;
    this->storage = &value;
    return *this;
  }

  Argument&StoreValues(std::vector<Type>& value) {
    this->has_storage = true;
    this->multi_storage = &value;
    return *this;
  }

  Argument& MultiValue() {
    this->is_multi_value = true;
    return *this;
  }

  Argument& MultiValue(int value) {
    this->is_multi_value = true;
    this->min_args_count = value;
    return *this;
  }

  Argument& Positional(){
    this->is_positional = true;
    return *this;
  }
};

class ArgParser {
 public:
  ArgParser(const std::string& name)
      : program_name(name) {}

  Argument<std::string>& AddStringArgument(const std::string& arg);
  Argument<std::string>& AddStringArgument(const char& short_arg, const std::string& arg);
  Argument<std::string>& AddStringArgument(const std::string& arg,
                               const std::string& description);
  Argument<std::string>& AddStringArgument(const char& short_arg, const std::string& arg,
                               const std::string& description);

  Argument<int>& AddIntArgument(const std::string& arg);
  Argument<int>& AddIntArgument(const char& short_arg, const std::string& arg);
  Argument<int>& AddIntArgument(const std::string& arg,
                            const std::string& description);
  Argument<int>& AddIntArgument(const char& short_arg, const std::string& arg,
                            const std::string& description);

  Argument<bool>& AddFlag(const std::string& arg);
  Argument<bool>& AddFlag(const char& short_arg, const std::string& arg);
  Argument<bool>& AddFlag(const std::string& arg, const std::string& description);
  Argument<bool>& AddFlag(const char& short_arg, const std::string& arg,
                     const std::string& description);


  bool Parse(const std::vector<std::string>& args);
  bool Parse(int& argc, char** argv);
  
  std::string GetValue(const std::string& value);
  std::string GetArgument(const std::string& arg, bool* is_equal_mark);
  std::vector<std::string> SplitFlagArgument(const std::string& arg);

  bool ParseStringArgument(int& i, int& size, std::string& arg, 
  std::string& value, bool* is_equal_mark);
  bool ParseMultiStringArgument(int& i, int& arg_size, std::string& arg, 
  std::string& value, bool* is_equal_mark);

  bool ParseIntArgument(int& i, int& size, std::string& arg, 
  std::string& value, bool* is_equal_mark);
  bool ParseMultiIntArgument(int& i, int& arg_size, std::string& arg, 
  std::string& value, bool* is_equal_mark);

  void ParseFlags(std::string& arg);

  std::string GetStringValue(const std::string& arg);
  std::string GetStringValue(const std::string& arg, const int& index);
  int GetIntValue(const std::string& arg);
  int GetIntValue(const std::string& arg, const int& index);
  bool GetFlag(const std::string& arg);

  ArgParser& AddHelp(const char& short_arg, const std::string& arg,
                     const std::string& description);
  bool Help();
  std::string HelpDescription();
  std::string HelpStringDescription();
  std::string HelpIntDescription();
  std::string HelpFlagDescription();

 private:
  std::string program_name;
  bool show_help = false;
  bool is_string_default = false;
  bool is_int_default = false;
  std::string current_arg = "";
  std::string help_arg = "";
  std::string short_help_arg = "";
  std::string program_description = "";
  std::string positional_arg = "";
  std::unordered_map<std::string, bool> is_enough_values;

  std::unordered_map<std::string, Argument<std::string>> string_args;
  std::unordered_map<std::string, Argument<int>> int_args;
  std::unordered_map<std::string, Argument<bool>> flags;
};

}  // namespace ArgumentParser