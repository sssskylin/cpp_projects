add_library(processing request_processing.cpp request_processing.h cache_processing.cpp cache_processing.h)
add_library(printer printer.cpp printer.h)
target_link_libraries(processing PUBLIC cpr::cpr nlohmann_json::nlohmann_json)
target_link_libraries(printer PUBLIC ftxui::screen ftxui::dom ftxui::component nlohmann_json::nlohmann_json)