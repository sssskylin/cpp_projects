#include "board.h"
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <cstdlib>
#include <ctime>

void Board::CleanShips() {
    for (auto ship : all_ships) {
        delete ship;
    }
}

void Board::PlaceOneDeckersRandom(uint64_t& x, uint64_t& y) {
  if (y) {
    x = x + 2;
  }
  uint64_t min_x = x;
  uint64_t min_y = y;
  uint8_t len = 1;
  while (amount_of_one_deckers > 0) {
    x = min_x + std::rand() % (width - min_x);
    y = std::rand() % height;
    while (!CheckHorizontalArrangement(x, y, len)) {
      x = min_x + std::rand() % (width - min_x);
      y = std::rand() % height;
    }
    SetOneDecker(x, y);
  }
}

void Board::SetOneDecker(uint64_t& x, uint64_t& y) {
  --amount_of_one_deckers;
  Ship* ship = new Ship();
  ship->condition = 1;
  ship->length = 1;
  ship->direction = 'v';
  ship->x = x;
  ship->y = y;
  all_ships.emplace_back(ship);
  std::pair<uint64_t, uint64_t> p(x, y);
  my_coordinates[p] = ship;
}

bool Board::CheckDensity(uint64_t& ship_area){
  if (height * width / 3 <= ship_area) {
    return true;
  }
  return false;
}