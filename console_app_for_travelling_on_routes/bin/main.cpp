#include "../lib/request_processing.h"
#include "../lib/cache_processing.h"
#include "../lib/printer.h"

int main(int argc, char** argv) {
  if (argc > 3) {
    std::cerr << "Too many args!!";
    return 1;
  }

  std::string first_city_code = "c2";
  std::string second_city_code = "c157";

  for (int i = 1; i < argc; ++i) {
    std::string date;
    date = argv[i];
    if (date[4] != '-') {
      date.insert(4, 1, '-');
      date.insert(7, 1, '-');
    }
    std::string filename = date + "-from-"+first_city_code+"-to-"+second_city_code+".json";
    RequestHandler routes(date, filename, first_city_code, second_city_code);
    routes.DateProcessing();
    Printer route_printer;
    route_printer.SetRoute(routes.GetData());
    route_printer.Print();
    std::swap(first_city_code, second_city_code);
  }

  return 0;
}
