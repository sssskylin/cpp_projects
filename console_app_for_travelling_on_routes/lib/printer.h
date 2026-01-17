#include <ftxui/dom/elements.hpp>           // for text, vbox, hbox, etc.
#include <ftxui/screen/screen.hpp>          // for Screen
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

#pragma once

class Printer {
 public:
  Printer() {}
  void SetRoute(const nlohmann::json& data);
  void Print();
 private:
  void FillTransportBuffer();
  void PrintOnScreen(ftxui::Element data);
  void PrintPath(std::string& from, 
                 std::string& to, 
                 std::string& departure_time, 
                 std::string& arrival_time, 
                 std::string& flight_number, 
                 std::string& transport);
  void PrintPartOfRoute(const int index);
  void PrintRoutes();
  nlohmann::json routes_;
  std::unordered_map<std::string, ftxui::Element> transport_buffer_;
};