add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(labwork6 PRIVATE cpr::cpr nlohmann_json::nlohmann_json processing)
target_link_libraries(labwork6 PRIVATE ftxui::screen ftxui::dom ftxui::component printer)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
