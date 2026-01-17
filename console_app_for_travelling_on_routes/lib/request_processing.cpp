#include "request_processing.h"
#include <vector>

RequestHandler::RequestHandler(const std::string& date,
                               const std::string& filename, 
                               const std::string& from, 
                               const std::string& to) {
  date_ = date; 
  filename_ = filename;
  from_ = from; 
  to_ = to;
}

void RequestHandler::DateProcessing() {
  cache_.SetDate(date_);
  cache_.SetFilename(filename_);
  if (CheckCache()) {
    routes_ = cache_.LoadCache();
  } else {
    CreateFile();
  }
}

nlohmann::json RequestHandler::GetData(){
  return routes_;
}

void RequestHandler::CreateFile() {

  cpr::Parameters parameters = cpr::Parameters{{"apikey", "123d08e3-f345-463d-8e0b-bd8c1ade26d7"},
                                               {"from", from_},
                                               {"to", to_},
                                               {"format", "json"},
                                               {"date", date_},
                                               {"transfers", "true"}};
  cpr::Response request = cpr::Get(cpr::Url{"https://api.rasp.yandex.net/v3.0/search/"},
                                  parameters);

  if (request.status_code != 200) {
    std::cerr << "Invalid request " << request.status_code << std::endl;
    exit(1);
    return;
  }

  routes_ = nlohmann::json::parse(request.text);
  std::vector<nlohmann::json> filtered_routes;
  
  for (const auto& segment : routes_["segments"]) {
    if (!segment.contains("transport_types") || (segment["transport_types"].size() <= 2)) {
      filtered_routes.emplace_back(segment);
    }
  }

  auto now = std::chrono::system_clock::now();
  std::time_t time_now = std::chrono::system_clock::to_time_t(now);
  long long last_updated = static_cast<long long>(time_now);

  routes_["segments"] = filtered_routes;
  routes_["last_updated"] = last_updated;

  cache_.SaveCache(routes_);
}

bool RequestHandler::CheckCache() {

  nlohmann::json cache_data = cache_.LoadCache();
  std::chrono::system_clock::time_point last_updated;

  if (!cache_data.is_null() && cache_data.contains("last_updated")) {
    last_updated = std::chrono::system_clock::time_point(
        std::chrono::seconds(cache_data["last_updated"].get<long long>()));
    if (!cache_.IsCacheExpired(last_updated, 3600)) {
      routes_ = cache_data;
      return true;
    }
  }
  return false;
}