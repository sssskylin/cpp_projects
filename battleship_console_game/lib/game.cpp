#include "game.h"

void Game::CreateRules() {
  new_board.width = 10000;
  new_board.height = 10000;
  new_board.amount_of_one_deckers = 1 + std::rand() % 239;
}

void Game::SetShips() {
  if (is_finished || is_stopped) {
    return;
  }
  CountShipArea();
  std::ifstream input(path_to_download_ships);
  uint64_t x = 0;
  uint64_t y = 0;
  if (input.is_open()) {
    uint8_t length;
    char direction;
    input >> new_board.width >> new_board.height;
    while (input >> length >> direction >> x >> y) {
      Ship* ship = new Ship();
      ship->condition = length;
      ship->length = length;
      ship->direction = direction;
      new_board.all_ships.emplace_back(ship);
      for (int i = 0; i < length; ++i) {
        std::pair<uint64_t, uint64_t> p(x, y);
        new_board.my_coordinates[p] = ship;
      }
      if (direction == 'v') {
        ++y;
      }
      if (direction == 'h') {
        ++x;
      }
      if (direction == 'v') {
        new_board.CheckVerticalArrangement(x, y, length);
      } 
      if (direction == 'h') {
        new_board.CheckHorizontalArrangement(x, y, length);
      }
    }
  } else {
    if (new_board.height > new_board.width) {
      new_board.PlaceVerticalFourDeckers(x, y);
      new_board.PlaceVerticalThreeDeckers(x, y);
      new_board.PlaceVerticalTwoDeckers(x, y);
      if (new_board.CheckDensity(ship_area)) {
        new_board.PlaceVerticalOneDeckers(x, y);
      } else {
        new_board.PlaceOneDeckersRandom(x, y);
      }
    } else {
      new_board.PlaceHorizontalFourDeckers(x, y);
      new_board.PlaceHorizontalThreeDeckers(x, y);
      new_board.PlaceHorizontalTwoDeckers(x, y);
      if (new_board.CheckDensity(ship_area)) {
        new_board.PlaceHorizontalOneDeckers(x, y);
      } else {
        new_board.PlaceOneDeckersRandom(x, y);
      }
    }
  }
}

void Game::CountShipArea() {
  ship_area = new_board.amount_of_one_deckers;
  ship_area += new_board.amount_of_two_deckers * 2;
  ship_area += new_board.amount_of_three_deckers * 3;
  ship_area += new_board.amount_of_four_deckers * 4;
}

void Game::Shot() {
  if (is_finished || is_stopped) {
    return;
  }
  if (strategy == "ordered") {
    last_x = ordered_strategy.x;
    last_y = ordered_strategy.y;
    ordered_strategy.Change(new_board.width);
  } 
  if (strategy == "custom") {
    custom_strategy.Change(new_board, last_result, shot_cells);
    last_x = custom_strategy.x;
    last_y = custom_strategy.y;
  }
  int n = new_board.width;
  int m = new_board.height;
  std::vector<std::vector<char>> matrix(m, std::vector<char>(n));
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      std::pair<uint64_t, uint64_t> p(j, i);
      if (shot_cells.count(p)){
        std::cout << '!' << " ";
      } else if (new_board.my_coordinates.count(p)) std::cout << 'X' << " ";
      else std::cout << '.' << " ";
    }
    std::cout << std::endl;
  }
}

void Game::SetResult(uint64_t& x, uint64_t& y) {
  if (is_finished || is_stopped) {
    return;
  }
  std::pair<uint64_t, uint64_t> p(x, y);
  shot_cells.insert(p);
  if (last_result == "miss") {
    return;
  }
  if (last_result == "hit") {
    FillEnemyShip(p);
    if (custom_strategy.last_dir != "") {
      ++custom_strategy.hit_counter;
    }
    custom_strategy.killer_mode = true;
    --ship_area;
  }
  if (last_result == "kill") {
    FillEnemyShip(p);
    custom_strategy.hit_counter = 0;
    custom_strategy.killer_mode = false;
    custom_strategy.last_dir = "";
    enemy_ship.is_first_empty = true;
    enemy_ship.is_last_empty = true;
    x = enemy_ship.first_cell.first;
    y = enemy_ship.first_cell.second;
    if (enemy_ship.first_cell.first == enemy_ship.last_cell.first) {
      enemy_ship.length = enemy_ship.last_cell.second - enemy_ship.first_cell.second + 1;
      VerticalTraversal(x, y, enemy_ship.length);
    } else if (enemy_ship.first_cell.second == enemy_ship.last_cell.second) {
      enemy_ship.length = enemy_ship.last_cell.first - enemy_ship.first_cell.first + 1;
      HorizontalTraversal(x, y, enemy_ship.length);
    }
  }
}

void Game::FillEnemyShip(std::pair<uint64_t, uint64_t>& p) {
  if (enemy_ship.is_first_empty) {
        enemy_ship.first_cell = p;
        enemy_ship.last_cell = p;
        enemy_ship.is_first_empty = false;
        enemy_ship.is_last_empty = false;
    } else {
        if (p.first < enemy_ship.first_cell.first || 
            (p.first == enemy_ship.first_cell.first && p.second < enemy_ship.first_cell.second)) {
            enemy_ship.first_cell = p;
        }

        if (p.first > enemy_ship.last_cell.first || 
            (p.first == enemy_ship.last_cell.first && p.second > enemy_ship.last_cell.second)) {
            enemy_ship.last_cell = p;
        }
    }
}

void Game::VerticalTraversal(uint64_t& x, uint64_t& y, short& length) {
  if (y != 0) {
    --y;
  std::pair<uint64_t, uint64_t> p(x, y);
  shot_cells.insert(p);
  }
  if (x + 1 != new_board.width) {
    ++x;
    if (y == 0) {
      for (int i = 0; i < length; ++i) {
        std::pair<uint64_t, uint64_t> p(x, y);
        shot_cells.insert(p);
        if (i != length - 1) ++y;
      }
    } else {
      for (int i = 0; i < length + 1; ++i) {
        std::pair<uint64_t, uint64_t> p(x, y);
        shot_cells.insert(p);
        if (i != length) ++y;
      }
    }
  }
  if (y + 1 != new_board.height) {
    ++y;
    std::pair<uint64_t, uint64_t> p(x, y);
    shot_cells.insert(p);
    --x;
    p = std::make_pair(x, y);
    shot_cells.insert(p);
  }
  if (x != 0) {
    --x;
    for (int i = 0; i < length + 1; ++i) {
      std::pair<uint64_t, uint64_t> p(x, y);
      shot_cells.insert(p);
      if (i != length) --y;
    }
    if (y != 0) {
      --y;
      std::pair<uint64_t, uint64_t> p(x, y);
      shot_cells.insert(p);
    }
  }
}

void Game::HorizontalTraversal(uint64_t& x, uint64_t& y, short& length) {
  if (x != 0) {
    --x;
    std::pair<uint64_t, uint64_t> p(x, y);
    shot_cells.insert(p);
  }
  if (y != 0) {
    --y;
    if (x == 0) {
      for (int i = 0; i < length; ++i) {
      std::pair<uint64_t, uint64_t> p(x, y);
      shot_cells.insert(p);
      if (i != length - 1) ++x;
    }
    } else {
      for (int i = 0; i < length + 1; ++i) {
      std::pair<uint64_t, uint64_t> p(x, y);
      shot_cells.insert(p);
      if (i != length) ++x;
    }
    }
  }
  if (x + 1 != new_board.width) {
    ++x;
    std::pair<uint64_t, uint64_t> p(x, y);
    shot_cells.insert(p);
    ++y;
    p = std::make_pair(x, y);
    shot_cells.insert(p);
  }
  if (y + 1 != new_board.height) {
    ++y;
    for (int i = 0; i < length + 1; ++i) {
      std::pair<uint64_t, uint64_t> p(x, y);
      shot_cells.insert(p);
      if (i != length) --x;
    }
    if (x != 0) {
      --x;
      std::pair<uint64_t, uint64_t> p(x, y);
      shot_cells.insert(p);
    }
  }
}

void Game::GetShot(uint64_t& x, uint64_t& y) {
  if (is_finished || is_stopped) {
    return;
  }
  std::pair<uint64_t, uint64_t> p(x, y);
  if (!new_board.my_coordinates.count(p)) {
    last_result = "miss";
  } else {
    --new_board.my_coordinates[p]->condition;
    if (new_board.my_coordinates[p]->condition > 0) {
      last_result = "hit";
    } else {
      last_result = "kill";
    }
  }
}

void Game::DumpShips(std::string& path_to_save_ships) {
  std::ofstream outFile(path_to_save_ships);
    if (outFile) {
    outFile << new_board.width << " " << new_board.height << "\n";
    for (size_t i = 0; i < new_board.all_ships.size(); ++i) {
      if ((i & 3) == 0) {
        outFile << new_board.all_ships[i]->length;
      } else if ((i & 3) == 1) {
        outFile << new_board.all_ships[i]->direction;
      } else if ((i & 3) == 2) {
        outFile << new_board.all_ships[i]->x;
      } else if ((i & 3) == 3) {
        outFile << new_board.all_ships[i]->y;
        outFile << "\n";
      } else {
        outFile << " ";
      }
    }
  }
  outFile.close();
}
