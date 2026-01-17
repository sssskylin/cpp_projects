#include "ArgParser.h"
#include <sstream>
#include <fstream>

#include <iostream>

using namespace ArgumentParser;

Argument<std::string>& ArgParser::AddStringArgument(const std::string& arg) {
  Argument<std::string> argument(arg);
  string_args[argument.arg] = argument;
  current_arg = arg;
  return string_args[arg];
}

Argument<std::string>& ArgParser::AddStringArgument(const char& short_arg, const std::string& arg) {
  std::string short_arg_str(1, short_arg);
  Argument<std::string> argument(short_arg_str, arg, 0);
  string_args[argument.arg] = argument;
  string_args[argument.short_arg] = argument;
  current_arg = arg;
  return string_args[arg];
}

Argument<std::string>& ArgParser::AddStringArgument(const std::string& arg, const std::string& description) {
  Argument<std::string> argument(arg, description, 1);
  string_args[argument.arg] = argument;
  current_arg = arg;
  return string_args[arg];
}

Argument<std::string>& ArgParser::AddStringArgument(const char& short_arg, const std::string& arg, const std::string& description) {
  std::string short_arg_str(1, short_arg);
  Argument<std::string> argument(short_arg_str, arg, description);
  string_args[argument.arg] = argument;
  string_args[argument.short_arg] = argument;
  current_arg = arg;
  return string_args[arg];
}

Argument<int>& ArgParser::AddIntArgument(const std::string& arg) {
  Argument<int> argument(arg);
  int_args[argument.arg] = argument;
  current_arg = arg;
  return int_args[arg];
}

Argument<int>& ArgParser::AddIntArgument(const char& short_arg, const std::string& arg) {
  std::string short_arg_str(1, short_arg);
  Argument<int> argument(short_arg_str, arg, 0);
  int_args[argument.arg] = argument;
  int_args[argument.short_arg] = argument;
  current_arg = arg;
  return int_args[arg];
}

Argument<int>& ArgParser::AddIntArgument(const std::string& arg, const std::string& description) {
  Argument<int> argument(arg, description, 1);
  int_args[argument.arg] = argument;
  current_arg = arg;
  return int_args[arg];
}

Argument<int>& ArgParser::AddIntArgument(const char& short_arg, const std::string& arg, const std::string& description) {
  std::string short_arg_str(1, short_arg);
  Argument<int> argument(short_arg_str, arg, description);
  int_args[argument.arg] = argument;
  int_args[argument.short_arg] = argument;
  current_arg = arg;
  return int_args[arg];
}

Argument<bool>& ArgParser::AddFlag(const std::string& arg) {
  Argument<bool> argument(arg);
  flags[argument.arg] = argument;
  current_arg = arg;
  return flags[arg];
}

Argument<bool>& ArgParser::AddFlag(const char& short_arg, const std::string& arg) {
  std::string short_arg_str(1, short_arg);
  Argument<bool> argument(short_arg_str, arg, 0);
  flags[argument.arg] = argument;
  flags[argument.short_arg] = argument;
  current_arg = arg;
  return flags[arg];
}

Argument<bool>& ArgParser::AddFlag(const std::string& arg, const std::string& description) {
  Argument<bool> argument(arg, description, 1);
  flags[argument.arg] = argument;
  current_arg = arg;
  return flags[arg];
}

Argument<bool>& ArgParser::AddFlag(const char& short_arg, const std::string& arg, const std::string& description) {
  std::string short_arg_str(1, short_arg);
  Argument<bool> argument(short_arg_str, arg, description);
  flags[argument.arg] = argument;
  flags[argument.short_arg] = argument;
  current_arg = arg;
  return flags[arg];
}

bool ArgParser::Parse(const std::vector<std::string>& args) {
  int arg_size = args.size();
  for (const auto& arg : string_args) {
    if (arg.second.has_default) {
      is_string_default = true;
    }
  }
  if (arg_size == 1 && string_args.size() > 0 && !is_string_default) {
    return false;
  }
  for (const auto& arg : int_args) {
    if (arg.second.has_default) {
      is_int_default = true;
    }
  }
  if (arg_size == 1 && int_args.size() > 0 && !is_int_default) {
    return false;
  }
  bool res = true;
  std::string value;
  bool positional_type = 0;
  for (const auto& arg : string_args) {
    if (arg.second.is_positional) {
      positional_arg = arg.first;
    }
  }
  for (const auto& arg : int_args) {
    if (arg.second.is_positional) {
      positional_arg = arg.first;
      positional_type = 1;
    }
  }

  for (int i = 1; i < args.size(); ++i) {
    bool is_equal_mark = false;
    std::string arg = GetArgument(args[i], &is_equal_mark);
    std::string prev_arg = GetArgument(args[i-1], &is_equal_mark);

    if (args[i-1][0] != '-' && args[i][0] != '-' && positional_arg == ""){
      return false;
    }

    if (i != arg_size - 1){
      value = args[i+1];
    }

    if (!positional_type && positional_arg != "" && !string_args.count(prev_arg) && !string_args.count(arg) && !flags.count(arg)){
      value = arg;
      arg = positional_arg;
    }
    if (positional_type && positional_arg != "" && !int_args.count(prev_arg) && !int_args.count(arg) && !flags.count(arg)){
      value = arg;
      arg = positional_arg;
    }

    if (is_equal_mark == true) {
      value = GetValue(args[i]);
    }
    if (help_arg == arg || short_help_arg == arg) {
      show_help = true;
    } else if (string_args.count(arg) && !string_args[arg].is_multi_value) {
      if (!ParseStringArgument(i, arg_size, arg, value, &is_equal_mark)) {
        return false;
      }
    } else if (string_args.count(arg) && string_args[arg].is_multi_value) {
      if (!ParseMultiStringArgument(i, arg_size, arg, value, &is_equal_mark)) {
        return false;
      }
    } else if (int_args.count(arg) && !int_args[arg].is_multi_value) {
      if (!ParseIntArgument(i, arg_size, arg, value, &is_equal_mark)) {
        return false;
      }
    } else if (int_args.count(arg) && int_args[arg].is_multi_value) {
      if (!ParseMultiIntArgument(i, arg_size, arg, value, &is_equal_mark)) {
        return false;
      }
    } else if (args[i][0] == '-') {
      ParseFlags(arg);
    }
  }
  for (const auto& val : is_enough_values) {
    if (val.second == false){
      return false;
    }
  }
  return res;
}

bool ArgParser::Parse(int& argc, char** argv) {
  std::vector<std::string> args;
  for (int i = 0; i < argc; ++i){
    std::string str_arg = std::string(argv[i]);
    args.emplace_back(str_arg);
  }
  return Parse(args);
}

bool ArgParser::ParseStringArgument(int& i, int& arg_size, 
std::string& arg, std::string& value, bool* is_equal_mark) {
  std::string arg1 = string_args[arg].arg;
  if ((i + 1 < arg_size && positional_arg != arg1) || positional_arg == arg1 || *is_equal_mark) {
    string_args[arg1].value = value;
    if (string_args[arg1].has_storage) {
      *string_args[arg1].storage = value;
    }
    return true;
  } else {
    return false;
  }
}

bool ArgParser::ParseMultiStringArgument(int& i, int& arg_size, 
std::string& arg, std::string& value, bool* is_equal_mark) {
  std::string arg1 = string_args[arg].arg;
  if ((i + 1 < arg_size && positional_arg != arg1) || positional_arg == arg1 || *is_equal_mark) {
    string_args[arg1].values.emplace_back(value);
    is_enough_values[arg1] = true;
    bool cond1 = string_args[arg1].min_args_count != 0;
    bool cond2 = string_args[arg1].min_args_count > string_args[arg1].values.size();
    if (cond1 && cond2){
      is_enough_values[arg1] = false;
    }
    if (string_args[arg1].has_storage) {
      std::vector<std::string> temp = *string_args[arg1].multi_storage;
      temp.emplace_back(value);
      *string_args[arg1].multi_storage = temp;
    }
    return true;
  } else {
    return false;
  }
}

bool ArgParser::ParseIntArgument(int& i, int& arg_size, 
std::string& arg, std::string& value, bool* is_equal_mark) {
  std::string arg1 = int_args[arg].arg;
  if ((i + 1 < arg_size && positional_arg != arg1) || positional_arg == arg1 || *is_equal_mark) {
    int_args[arg1].value = std::stoi(value);
    if (int_args[arg1].has_storage) {
      *int_args[arg1].storage = std::stoi(value);
    }
    return true;
  } else {
    return false;
  }
}

bool ArgParser::ParseMultiIntArgument(int& i, int& arg_size, std::string& arg, 
std::string& value, bool* is_equal_mark) {
  std::string arg1 = int_args[arg].arg;
  if ((i + 1 < arg_size && positional_arg != arg1) || positional_arg == arg1 || *is_equal_mark) {
    int_args[arg1].values.emplace_back(std::stoi(value));
    is_enough_values[arg1] = true;
    bool cond1 = int_args[arg1].min_args_count != 0;
    bool cond2 = int_args[arg1].min_args_count > int_args[arg1].values.size();
    if (cond1 && cond2){
      is_enough_values[arg1] = false;
    }
    if (int_args[arg1].has_storage) {
      std::vector<int> temp = *int_args[arg1].multi_storage;
      temp.emplace_back(std::stoi(value));
      *int_args[arg1].multi_storage = temp;
    }
    return true;
  } else {
    return false;
  }
}

void ArgParser::ParseFlags(std::string& arg) {
  std::vector<std::string> flag_args = SplitFlagArgument(arg);
  for (const auto& flag_arg : flag_args) {  
    if (flags.count(flag_arg)) {
      std::string arg1 = flags[flag_arg].arg;
      flags[arg1].value = true;
      if (flags[arg1].has_storage) {
        *flags[arg1].storage  = true;
      }
    }
  }
}

std::string ArgParser::GetValue(const std::string& value) {
  std::string res_value = "";
  int i = value.find('=') + 1;
  while (value[i] != '\0') {
    res_value = res_value + value[i];
    ++i;
  }
  return res_value;
}

std::string ArgParser::GetArgument(const std::string& arg, bool* is_equal_mark) {
  int i = 0;
  std::string res = "";
  while (arg[i] != '=' && arg[i] != '\0') {
    if (arg[i] != '-') {
      res = res + arg[i];
    }
    ++i;
    if (arg[i] == '=') {
      *is_equal_mark = true;
    }
  }
  return res;
}

std::vector<std::string> ArgParser::SplitFlagArgument(const std::string& arg){
  int i = 0;
  std::vector<std::string> res_args;
  std::string str = "";
  while (arg[i] != '\0') {
    if (arg[i] != '-') {
      str = str + arg[i];
    }
    if (flags.count(str) || arg[i+1] == '\0'){
      res_args.emplace_back(str);
      str = "";
    }
    ++i;
  }
  return res_args;
}

std::string ArgParser::GetStringValue(const std::string& arg) {
  std::string res_arg = string_args[arg].arg;
  return string_args[res_arg].value;
}

std::string ArgParser::GetStringValue(const std::string& arg, const int& index) {
  std::string res_arg = string_args[arg].arg;
  return string_args[res_arg].values[index];
}

int ArgParser::GetIntValue(const std::string& arg) {
  std::string res_arg = int_args[arg].arg;
  return int_args[res_arg].value;
}

int ArgParser::GetIntValue(const std::string& arg, const int& index) {
  std::string res_arg = int_args[arg].arg;
  return int_args[res_arg].values[index];
}

bool ArgParser::GetFlag(const std::string& arg) {
  std::string res_arg = flags[arg].arg;
  return flags[res_arg].value;
}

ArgParser& ArgParser::AddHelp(const char& short_arg, const std::string& arg, const std::string& description) {
  short_help_arg = short_arg;
  help_arg = arg;
  program_description = description;
  return *this; 
}

bool ArgParser::Help(){
  return show_help; 
}

std::string ArgParser::HelpStringDescription(){
  std::string result = "";
  for (const auto& arg : string_args) {
    if ( arg.first == arg.second.short_arg ){
      result = result + "-" + arg.second.short_arg + ",  ";
    } else {
      result = result + "     ";
    }
    result = result + "--" + arg.second.arg + "=<string>,  " + arg.second.description;
    if (arg.second.has_default){
      result = result + " [default = " + arg.second.default_value + "]";
    }
    if (arg.second.is_multi_value){
      result = result + " [repeated";
      if (arg.second.min_args_count != 0){
        result = result + ", min args = " + std::to_string(arg.second.min_args_count);
      }
      result = result + "]";
    }
    result = result + "\n";
  }
  return result;
}

std::string ArgParser::HelpIntDescription(){
  std::string result = "";
  for (const auto& arg : int_args) {
    if (arg.first == arg.second.short_arg){
      result = result + "-" + arg.second.short_arg + ",  ";
    } else {
      result = result + "     ";
    }
    result = result + "--" + arg.second.arg + "=<int>,  " + arg.second.description;
    if (arg.second.has_default){
      result = result + " [default = " + std::to_string(arg.second.default_value) + "]";
    }
    if (arg.second.is_multi_value){
      result = result + " [repeated";
      if (arg.second.min_args_count != 0){
        result = result + ", min args = " + std::to_string(arg.second.min_args_count);
      }
      result = result + "]";
    }
    result = result + "\n";
  }
  return result;
}

std::string ArgParser::HelpFlagDescription(){
  std::string result = "";
  for (const auto& arg : flags) {
    if (arg.first == arg.second.short_arg){
      result = result + "-" + arg.second.short_arg + ",  ";
    } else {
      result = result + "     ";
    }
    result = result + "--" + arg.second.arg + ",  " + arg.second.description;
    if (arg.second.has_default){
      result = result + " [default = ";
      if (arg.second.default_value){
        result = result + "true";
      } else {
        result = result + "false";
      }
      result = result + "]";
    }
    result = result + "\n";
  }
  return result;
}

std::string ArgParser::HelpDescription(){
  std::string result = 
    program_name + "\n" +
    program_description + "\n"
    "\n" + 
    HelpStringDescription() +
    HelpIntDescription() +
    HelpFlagDescription();
    if (short_help_arg != "" || help_arg != ""){
      result = result + "-" + short_help_arg + ",  --" + help_arg + " Display this help and exit\n";
    }
  return result;
}
