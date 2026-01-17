#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

bool Status(std::string str) {  // функция узнает равен ли статус ошибки 500
  bool status = false;
  std::string error_status;
  int space = 0;

  for (int j = str.size() - 1; j > 0; --j) {
    if (str[j] == '\"') {
      space = j + 2;
      break;
    }
  }

  for (int error = space; error < space + 3; ++error) {
    error_status += str[error];
  }
  if (error_status[0] == '5') {
    status = true;
  }
  return status;
}

std::string URLFinder(std::string str) {  // находит URL запроса
  int left_space = 0;
  int right_space = 0;
  int start = 0;
  int end = str.size() - 1;

  while (left_space < 5) {
    ++start;
    if (str[start] == ' ') {
      left_space += 1;
    }
  }
  while (right_space < 2) {
    --end;
    if (str[end] == ' ') {
      right_space += 1;
    }
  }
  std::string url;
  for (int i = start + 1; i < end; ++i) {
    url += str[i];
  }
  return url;
}

int DateToTimestamp(std::string str) {  // перевод даты в формат Timestamp
  int date_start = 0;
  int date_end = 0;
  std::string time;
  for (int i = 0; i < str.size(); ++i) {
    if (str[i] == ']') {
      date_end = 1;
    }
    if ((date_start != 0) and (date_end == 0)) {
      time += str[i];
    }
    if (str[i] == '[') {
      date_start = 1;
    }
  }
  int day = std::stoi(time.substr(0, 2)) - 1;

  std::string mounth = time.substr(3, 3);

  int year = std::stoi(time.substr(7, 4));

  int hour = std::stoi(time.substr(12, 2));

  int minute = std::stoi(time.substr(15, 2));

  int second = std::stoi(time.substr(18, 2));

  int time_zone_hour = std::stoi(time.substr(22, 2));

  int time_zone_minute = std::stoi(time.substr(24, 2));

  int year_difference = year - 1970;

  int leap_year;

  if (year_difference % 4 > 1) {
    leap_year = year_difference / 4 + 1;
  } else {
    leap_year = year_difference / 4;
  }

  std::string months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  int days_in_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  int index = 0;
  int days = 0;
  while (months[index] != mounth) {
    days += days_in_month[index];
    ++index;
  }
  int timestamp = year_difference * 365 * 86400 + leap_year * 86400 +
                  days * 86400 + day * 86400 + hour * 3600 + minute * 60 +
                  second;
  if (time[21] == '-') {
    timestamp += time_zone_hour * 3600 + time_zone_minute * 60;
  } else {
    timestamp -= time_zone_hour * 3600 - time_zone_minute * 60;
  }

  return (timestamp);
}

int main(int argc, char** argv) {
  std::string file_name = "";    // access.log
  std::string output_file = "";  // результирующий файл
  for (int i = 1; i < argc;
       ++i) {  // находим имя файла, который будем считывать
    std::string str_argv = std::string(argv[i]);
    if ((str_argv[0] != '-') and (str_argv != "AnanlyzeLog") and
        (isdigit(str_argv[0]) == false)) {
      file_name = str_argv;
    }
  }
  for (int i = 1; i < argc; ++i) {  // находим имя файла, в который будем
                                    // записывать логи с кодом ошибки 5ХХ
    std::string str_argv = std::string(argv[i]);
    if (str_argv.substr(0, 9) == "--output=") {
      output_file = str_argv.substr(9, str_argv.size() - 9);
    } else if ((str_argv == "-o") or (str_argv == "-output")) {
      if (((i + 1) != argc) and (argv[++i][0] != '-')) {
        output_file = argv[++i];
      }
    }
  }
  bool wrong_argument = false;
  for (int i = 2; i < argc;
       ++i) {  // проверяем, есть ли ошибка в вводе аргументов
    std::string str_argv = std::string(argv[i]);
    if ((isdigit(str_argv[0]) == false) and (str_argv != file_name) &&
        (str_argv != "-o") && (str_argv != "-p") && (str_argv != "-s") &&
        (str_argv != "-w") && (str_argv != "-f") && (str_argv != "-t") &&
        (str_argv != "--print") && (str_argv.substr(0, 9) != "--output=") &&
        (str_argv.substr(0, 8) != "--stats=") &&
        (str_argv.substr(0, 9) != "--window=") &&
        (str_argv.substr(0, 7) != "--from=") &&
        (str_argv.substr(0, 5) != "--to=") && (str_argv != "--output") &&
        (str_argv != "--stats") && (str_argv != "--window") &&
        (str_argv != "--from") && (str_argv != "--to")) {
      wrong_argument = true;
    }
  }
  if ((output_file == "") or (file_name == "")) {
    std::cerr << "Error, file name is not defined" << std::endl;
  } else if (wrong_argument == true) {
    std::cerr << "Error, argument is not correct" << std::endl;
  } else {
    int time_start = 0;
    int time_end = 0;

    for (int i = 1; i < argc; ++i) {
      std::string str_argv = std::string(argv[i]);

      if ((str_argv.substr(0, 7) == "--from=") or
          (str_argv == "-f")) {  // находим время, начиная с которого происходит
                                 // анализ данных
        if (str_argv.substr(0, 7) == "--from=") {
          time_start = std::stoi(str_argv.substr(7, str_argv.size() - 7));
        } else {
          time_start = std::stoi(std::string(argv[++i]));
        }
      }
      if ((str_argv.substr(0, 5) == "--to=") or
          (str_argv ==
           "-t")) {  // находим время, до которого происходит анализ данных
        if (str_argv.substr(0, 5) == "--to=") {
          time_end = std::stoi(str_argv.substr(5, str_argv.size() - 5));
        } else {
          time_end = std::stoi(std::string(argv[++i]));
        }
      }
    }
    if (time_end ==
        0) {  // если не было задано время, до которого происходит анализ данных
      std::ifstream file(file_name);
      std::string time;
      if (file.is_open()) {
        while (std::getline(file, time)) {
          time_end = DateToTimestamp(time);
        }
      }
      file.close();
    }

    int all_lines = 0;
    std::string Log;

    std::ofstream out(
        output_file);  // записываем ошибки с кодом 5ХХ в результирующий файл
    if (out.is_open()) {
      std::ifstream in(file_name);
      if (in.is_open()) {
        while (std::getline(in, Log)) {
          ++all_lines;
          if ((Status(Log) == true) and (DateToTimestamp(Log) >= time_start) and
              (DateToTimestamp(Log) <= time_end)) {
            out << Log << std::endl;
          }
        }
        in.close();
      }
      out.close();
    }

    for (int i = 1; i < argc; ++i) {
      std::string str_argv = std::string(argv[i]);
      std::string str_argv_1;
      if ((i + 1 != argc)) {
        str_argv_1 = std::string(argv[i + 1]);
      }
      if ((str_argv == "-p") ||
          (str_argv == "--print")) {  // организуем вывод ошибок в консоль
        std::string log;
        std::ifstream in(output_file);
        if (in.is_open()) {
          while (std::getline(in, log)) {
            std::cout << log << std::endl;
          }
        }
        in.close();
      }

      if ((str_argv == "-s") or (str_argv.substr(0, 8) == "--stats") or
          (str_argv.substr(0, 8) ==
           "--stats=")) {  // выводим самые частые запросы с кодом 5ХХ
        int stats_n;
        if (str_argv.substr(0, 8) == "--stats=") {
          stats_n = std::stoi(str_argv.substr(8, str_argv.size() - 8));
        } else if ((i + 1 != argc)) {
          if ((str_argv_1 != file_name) and (argv[++i][0] != '-')) {
            stats_n = std::stoi(str_argv_1);
          }
        } else {
          stats_n = 10;
        }
        std::unordered_map<std::string, int> error_counter_500;
        std::string Log;

        std::ofstream out(output_file);
        if (out.is_open()) {
          std::ifstream in(file_name);
          if (in.is_open()) {
            while (std::getline(in, Log)) {
              if ((Status(Log) == true) and
                  (DateToTimestamp(Log) >= time_start) and
                  (DateToTimestamp(Log) <= time_end)) {
                ++error_counter_500[URLFinder(Log)];
              }
            }
            in.close();
          }
          out.close();
        }

        for (int q = 0; q < stats_n; ++q) {
          int max_value = 0;

          for (auto error : error_counter_500) {
            if (error.second > max_value) {
              max_value = error.second;
            }
          }

          for (auto error : error_counter_500) {
            if (error.second == max_value) {
              std::cout << error.first << std::endl;
              error_counter_500.erase(error.first);
              break;
            }
          }
        }
      }
      if ((str_argv == "-w") or (str_argv == "--window") or
          (str_argv.substr(0, 9) == "--window=")) {  // находим промежуток, в который количество запросов
                            // было максимально.
        int window_time;
        if (str_argv.substr(0, 9) == "--window=") {
          window_time = std::stoi(str_argv.substr(9, str_argv.size() - 9));
        } else if ((i + 1 != argc)) {
          if ((str_argv_1 != file_name) and (argv[++i][0] != '-')) {
            window_time = std::stoi(str_argv_1);
          } else {
            window_time = 0;
          }
        }

        if (window_time != 0) {
          int log_list[all_lines];
          int max_requests = 0;
          int start_window_res = 0;
          int end_window_res = 0;
          int start_window = 0;
          int counter = 0;
          int a = 0;

          std::string Log;
          std::ifstream file(file_name);
          if (file.is_open()) {
            while (std::getline(file, Log)) {
              int time_log = DateToTimestamp(Log);
              if ((time_log >= time_start) and (time_log <= time_end)) {
                log_list[a] = time_log;
                ++a;
              }
            }
          }

          file.close();
          for (int end_window = 0; end_window < all_lines; ++end_window) {
            if (log_list[end_window] < log_list[start_window] + window_time) {
              ++counter;
            } else {
              if (counter > max_requests) {
                max_requests = counter;
                start_window_res = start_window;
                end_window_res = end_window;
              }
              ++start_window;
            }
          }

          std::cout << log_list[start_window_res] << ' '
                    << log_list[end_window_res] << ' ' << max_requests
                    << std::endl;
        }
      }
    }
  }
}

// я надеюсь боги си плюс плюс запустят этот код