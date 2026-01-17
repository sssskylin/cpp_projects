#include <processing.h>

#include <format>
#include <iostream>
#include <map>
#include <utility>

/*
Программа считает частоту слов, встречающихся во всех файлах в директории
и выводит итоговую статистику в консоль
*/

int main(int argc, char** argv) {
  if (argc != 2) return 1;

  bool recursive = false;
  Dir(argv[1], recursive) |
      Filter([](std::filesystem::path p) { return p.extension() == ".txt"; }) |
      OpenFiles() | Split("\n ,.;") | Transform([](std::string token) {
        std::transform(token.begin(), token.end(), token.begin(),
                       [](char c) { return std::tolower(c); });
        return token;
      }) |
      AggregateByKey(
          0uz, [](std::string, size_t count) { ++count; },
          [](std::string token) { return token; }) |
      Transform([](std::pair<std::string, size_t> stat) {
        return std::format("{} - {}", stat.first, stat.second);
      }) |
      Out(std::cout);
  return 0;

  const std::map<int, int> m = {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
  auto res = AsDataFlow(m) | Transform([](std::pair<int, int> x) {
               return std::make_pair(x.first, x.second + 50);
             }) |
             Filter([](std::pair<int, int> x) { return x.second % 2 == 0; });
  auto res2 = AsDataFlow(m) | Join(res);
  for (const auto& item : res2) {
    if (item.joined.has_value()) {
      std::cout << item.base << " " << item.joined.value() << std::endl;
    }
  }

  const std::vector<KV<int, int>> a = {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};

  auto flow = AsDataFlow(a) | Transform([](KV<int, int> num) {
                if (num.value % 2 == 0) {
                  num.value *= -1;
                }
                return num;
              }) |
              Filter([](KV<int, int> num) { return num.value < 0; });

  auto flow_two = AsDataFlow(a) | Join(flow);
  for (auto i : flow_two) {
    if (i.joined.has_value()) {
      std::cout << i.base << ' ' << i.joined.value() << std::endl;
    }
  }
}