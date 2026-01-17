#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include <chrono>
#include <ctime>

#pragma once

class CacheHandler {
 public:
  CacheHandler(){}

  void SetDate(const std::string& date);
  void SetFilename(const std::string& name);
  nlohmann::json LoadCache();
  void SaveCache(const nlohmann::json& data);
  bool IsCacheExpired(const std::chrono::system_clock::time_point& lastUpdated,
                      int expirationTimeInSeconds);
 private:
  std::string date;
  std::string filename;
};
