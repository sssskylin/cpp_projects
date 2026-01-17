#include "cache_processing.h"

void CacheHandler::SetDate(const std::string& date_arg) {
  date = date_arg;
}

void CacheHandler::SetFilename(const std::string& name) {
  filename = name;
}

nlohmann::json CacheHandler::LoadCache() {
  nlohmann::json cache_data;
  std::ifstream inFile(filename);
  if (inFile.is_open()) {
    inFile >> cache_data;
    inFile.close();
  }
  return cache_data;
}

void CacheHandler::SaveCache(const nlohmann::json& data) {
  std::ofstream outFile(filename);
  if (outFile.is_open()) {
    outFile << data.dump(2);
    outFile.close();
  }
}

bool CacheHandler::IsCacheExpired(const std::chrono::system_clock::time_point& last_updated,
                                  int expiration_time) {
  auto now = std::chrono::system_clock::now();
  return std::chrono::duration_cast<std::chrono::seconds>(now - last_updated).count() 
          > expiration_time;
}

