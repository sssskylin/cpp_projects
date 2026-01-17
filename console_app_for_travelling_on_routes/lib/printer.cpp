#include "printer.h"
#include <iostream>

void Printer::SetRoute(const nlohmann::json& data){
  routes_ = data;
}

void Printer::FillTransportBuffer() {

  auto plane = ftxui::vbox({
    ftxui::text("plane")  | ftxui::color(ftxui::Color::HotPink)  | ftxui::bold | ftxui::center,
    ftxui::text("      ______        ") | ftxui::color(ftxui::Color::SkyBlue1),
    ftxui::text("      \\\\   \\\\      ") | ftxui::color(ftxui::Color::SkyBlue1),
    ftxui::text("|\\\\____\\\\___\\\\__________ ") | ftxui::color(ftxui::Color::SkyBlue1),
    ftxui::text(" \\\\ . .  ______ . \\\\___\\\\ ") | ftxui::color(ftxui::Color::SkyBlue1),
    ftxui::text("  \\\\____//   //_________\\\\ ") | ftxui::color(ftxui::Color::SkyBlue1),
    ftxui::text("       //_._//              ") | ftxui::color(ftxui::Color::SkyBlue1)
  }) | ftxui::border;

  auto train = ftxui::vbox({
    ftxui::text("train")  | ftxui::color(ftxui::Color::HotPink)  | ftxui::bold | ftxui::center,
    ftxui::text(" _________   ______") | ftxui::color(ftxui::Color::LightCoral),
    ftxui::text("|| # # # || ||  \\_||__") | ftxui::color(ftxui::Color::Pink1),
    ftxui::text("||_______||-||_______||") | ftxui::color(ftxui::Color::Pink1),
    ftxui::text(" *   *   *   *   *   *") | ftxui::color(ftxui::Color::DeepSkyBlue1)
  }) | ftxui::border;

  auto suburban = ftxui::vbox({
    ftxui::text("suburban")  | ftxui::color(ftxui::Color::HotPink) | ftxui::bold | ftxui::center,
    ftxui::text("    ___	   ___") | ftxui::color(ftxui::Color::GreenYellow),
    ftxui::text(" _____/______/___") | ftxui::color(ftxui::Color::GreenYellow),
    ftxui::text("||  #  #  # |____\\\\") | ftxui::color(ftxui::Color::PaleGreen1),
    ftxui::text("||_______________||") | ftxui::color(ftxui::Color::PaleGreen1),
    ftxui::text(" *   *   *   *   *") | ftxui::color(ftxui::Color::HotPink)
  }) | ftxui::border;

  auto bus = ftxui::vbox({
    ftxui::text("bus")  | ftxui::color(ftxui::Color::HotPink) | ftxui::bold | ftxui::center,
    ftxui::text(" _________________") | ftxui::color(ftxui::Color::Yellow),
    ftxui::text("|| #   #   #  \\__||") | ftxui::color(ftxui::Color::Yellow),
    ftxui::text("||_______________||") | ftxui::color(ftxui::Color::Yellow),
    ftxui::text("   *   *   *   *   ") | ftxui::color(ftxui::Color::HotPink)
  }) | ftxui::border;

  transport_buffer_["plane"] = plane;
  transport_buffer_["train"] = train;
  transport_buffer_["suburban"] = suburban;
  transport_buffer_["bus"] = bus;
}

void Printer::PrintOnScreen(ftxui::Element data){
  auto screen = ftxui::Screen::Create(
    ftxui::Dimension::Full(),    // Ширина
    ftxui::Dimension::Fit(data) // Высота
  );

  ftxui::Render(screen, data);
  screen.Print();
}

void Printer::PrintPath(std::string& from, 
                        std::string& to, 
                        std::string& departure_time, 
                        std::string& arrival_time, 
                        std::string& flight_number, 
                        std::string& transport) {
  departure_time[10] = ' ';
  arrival_time[10] = ' ';

  auto departure = ftxui::vbox({
    ftxui::text("Departure:") | ftxui::bold | ftxui::color(ftxui::Color::PaleGreen1),
    ftxui::text(""),
    ftxui::text("City: " + from),
    ftxui::text("Time: " + departure_time),
    ftxui::text("Transport number: " + flight_number),
  }) | ftxui::border | ftxui::flex;

  auto arrival = ftxui::vbox({
    ftxui::text("Arrival:") | ftxui::bold | ftxui::color(ftxui::Color::PaleGreen1),
    ftxui::text(""),
    ftxui::text("City: " + to),
    ftxui::text("Time: " + arrival_time),
  }) | ftxui::border | ftxui::flex;

  auto path = ftxui::hbox (transport_buffer_[transport], departure, arrival);
  PrintOnScreen(path);
}

void Printer::PrintPartOfRoute(int index){

  bool has_transfers = routes_["segments"][index]["has_transfers"];

  if (!has_transfers) {
    std::string from = routes_["segments"][index]["from"]["title"]; 
    std::string to = routes_["segments"][index]["to"]["title"]; 
    std::string departure_time = routes_["segments"][index]["departure"];
    std::string arrival_time = routes_["segments"][index]["arrival"];
    std::string flight_number = routes_["segments"][index]["thread"]["number"];
    std::string transport = routes_["segments"][index]["thread"]["transport_type"];

    PrintPath(from, to, departure_time, arrival_time, flight_number, transport);
  } else {
    for (int i = 0; i < routes_["segments"][index]["details"].size(); i+=2) {
      std::string from = routes_["segments"][index]["details"][i]["from"]["title"]; 
      std::string to = routes_["segments"][index]["details"][i]["to"]["title"]; 
      std::string departure_time = routes_["segments"][index]["details"][i]["departure"]; 
      std::string arrival_time = routes_["segments"][index]["details"][i]["arrival"];
      std::string flight_number = routes_["segments"][index]["details"][i]["thread"]["number"];
      std::string transport = routes_["segments"][index]["details"][i]["thread"]["transport_type"];

      PrintPath(from, to, departure_time, arrival_time, flight_number, transport);
    }
  }
}

void Printer::PrintRoutes() {

  if (routes_["segments"].size() == 0){
    std::cerr << "Routes not found" << std::endl;
    exit(1);
    return;
  }

  int total_segments = routes_["pagination"]["total"];

  for (int i = 0; i < total_segments; ++i) {
    std::string way = "Route №" + std::to_string(i + 1);
    std::string has_transfers = "false";
    if (routes_["segments"][i]["has_transfers"]) {
      has_transfers = "true";
    }
    std::string transfers = "Has transfers: " + has_transfers;

    auto part_of_route = ftxui::hbox({
      ftxui::text(way) | ftxui::flex | ftxui::border | ftxui::bold,
      ftxui::text(transfers) | ftxui::flex | ftxui::border | ftxui::bold,
    });

    PrintOnScreen(part_of_route);
    PrintPartOfRoute(i);
  }
}

void Printer::Print() {

  FillTransportBuffer();

  std::string date = routes_["search"]["date"];
  std::string from = routes_["search"]["from"]["title"];
  std::string to = routes_["search"]["to"]["title"];

  auto info = ftxui::vbox({
    ftxui::text(date) | ftxui::color(ftxui::Color::HotPink) | ftxui::center,
    ftxui::text(from + " - " + to) | ftxui::bold | ftxui::center,
  }) | ftxui::border | ftxui::flex;

  PrintOnScreen(info);
  PrintRoutes();
}
