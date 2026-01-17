#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include "cache_processing.h"
#include <string>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iostream>

#pragma once

class RequestHandler {
 public:
  RequestHandler() {}
  RequestHandler(const std::string& date,
                 const std::string& filename, 
                 const std::string& from,
                 const std::string& to);
  void DateProcessing();
  nlohmann::json GetData();
 private:
  void CreateFile();
  bool CheckCache();
  std::string date_;
  std::string filename_;
  std::string from_;
  std::string to_;
  nlohmann::json routes_;
  CacheHandler cache_;
};