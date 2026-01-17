#include <iostream>
#include "../lib/board.h"
#include "../lib/game.h"
#include "../lib/strategy.h"
#include "../lib/parser.h"
#include <cstdlib>
#include <ctime>

int main() {
  Game new_game;
  std::srand(std::time(nullptr));
  while (true) {
    std::string cmd;
    std::getline(std::cin, cmd);

    std::vector<std::string> cmd_vector = SplitCommand(cmd);

    if (cmd == "exit") {
      std::cout << "ok" << "\n";
      return 0;
    } else {
      Parse(cmd_vector, new_game);
    }
  }
  return 0;
}