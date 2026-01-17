#include <sstream>
#include "parser.h"

std::vector<std::string> SplitCommand(const std::string& cmd) {
    std::istringstream iss(cmd);

    return {std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>()};
}

void Parse(std::vector<std::string>& cmd_vector, Game& new_game) {
    if (cmd_vector[0] == "ping") {
      std::cout << "pong" << std::endl;

    } else if (cmd_vector[0] == "create") {
      new_game.game_mode = cmd_vector[1];
      if (cmd_vector[1] == "master") {
        new_game.CreateRules();
      }
      std::cout << "ok" << "\n";

    } else if (cmd_vector[0] == "start") {
      new_game.is_started = true;
      new_game.is_stopped = false;
      new_game.is_finished = false;
      new_game.SetShips();
      std::cout << "ok" << "\n";
    } else if (cmd_vector[0] == "stop") {
      new_game.is_started = false;
      new_game.is_stopped = true;
      std::cout << "ok" << "\n";
    } else if (cmd_vector[0] == "set"){
      if (cmd_vector[1] == "strategy") {
        if (cmd_vector[2] == "ordered") {
          new_game.strategy = "ordered";
        } else if (cmd_vector[2] == "custom") {
          new_game.strategy = "custom";
        }
        std::cout << "ok" << "\n";
      } else if (cmd_vector[1] == "result") {
        new_game.last_result = cmd_vector[2];
        new_game.SetResult(new_game.last_x, new_game.last_y);
        std::cout << "ok" << "\n";
      } else {
        if (new_game.game_mode == "master") {
          std::cout << "failed" << "\n";
        } else {
          if (cmd_vector[1] == "width") {
          new_game.new_board.width = std::stoull(cmd_vector[2]);
          } else if (cmd_vector[1] == "height") {
            new_game.new_board.height = std::stoull(cmd_vector[2]);
          } else if (cmd_vector[1] == "count") {
            uint8_t decks = cmd_vector[2][0] - '0';
            new_game.custom_strategy.max_len = std::max(decks, new_game.custom_strategy.max_len);
            if (decks == 1) {
              new_game.new_board.amount_of_one_deckers =
                std::stoull(cmd_vector[3]);
            } else if (decks == 2) {
              new_game.new_board.amount_of_two_deckers =
                std::stoull(cmd_vector[3]);
            } else if (decks == 3) {
              new_game.new_board.amount_of_three_deckers =
                std::stoull(cmd_vector[3]);
            } else if (decks == 4) {
              new_game.new_board.amount_of_four_deckers =
                std::stoull(cmd_vector[3]);
            }
          }
          std::cout << "ok" << "\n";
        }
      }
    } else if (cmd_vector[0] == "get") {
      if (cmd_vector[1] == "width") {
        std::cout << new_game.new_board.width << std::endl;
      } else if (cmd_vector[1] == "height") {
        std::cout << new_game.new_board.height << std::endl;
      } else if (cmd_vector[1] ==  "count") {
        uint8_t decks = cmd_vector[2][0] - '0';
        if (decks == 1) {
          std::cout << new_game.new_board.amount_of_one_deckers << "\n";
        } else if (decks == 2) {
          std::cout << new_game.new_board.amount_of_two_deckers << "\n";
        } else if (decks == 3) {
          std::cout << new_game.new_board.amount_of_three_deckers << "\n";
        } else if (decks == 4) {
          std::cout << new_game.new_board.amount_of_four_deckers << "\n";
        }
      } else if (cmd_vector[1] ==  "str") {
        std::cout << new_game.strategy << "\n";
      }
    } else if (cmd_vector[0] == "shot") {
      if (cmd_vector.size() == 1) {
        new_game.Shot();
        std::cout << new_game.last_x << " " << new_game.last_y << "\n";
      } else {
        int i = 5;
        uint64_t x = std::stoull(cmd_vector[1]);
        uint64_t y = std::stoull(cmd_vector[2]);
        new_game.GetShot(x, y);
        std::cout << new_game.last_result << "\n";
      }
    } else if (cmd_vector[0] == "finished") {
      if (new_game.is_finished) {
        std::cout << "yes" << "\n";
      } else {
        std::cout << "no" << "\n";
      }
    } else if (cmd_vector[0] == "win") {
      if (new_game.ship_area == 0) {
        std::cout << "yes" << "\n";
      } else {
        std::cout << "no" << "\n";
      }
    } else if (cmd_vector[0] == "lose") {
      if (new_game.ship_area != 0) {
        std::cout << "yes" << "\n";
      } else {
        std::cout << "no" << "\n";
      }
    } else if (cmd_vector[0] == "dump") {
      std::string path_to_save_ships = cmd_vector[1];
      new_game.DumpShips(path_to_save_ships);
      std::cout << "ok" << "\n";
    } else if (cmd_vector[0] == "load") {
      new_game.path_to_download_ships = cmd_vector[1];
      std::cout << "ok" << "\n";
    }
}