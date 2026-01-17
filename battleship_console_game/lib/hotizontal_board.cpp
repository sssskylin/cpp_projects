#include "board.h"
#include <fstream>
#include <iostream>
#include <string>
#include <utility>

bool Board::CheckHorizontalShips(uint64_t& x, uint64_t& y) {
  if (x >= width) {
    return false;
  }
  return true;
}

void Board::PlaceHorizontalFourDeckers(uint64_t& x, uint64_t& y) {
  while (amount_of_four_deckers > 0) {
    if (x + 4 <= width) {
      SetHorizontalFourDecker(x, y);
    }
    if (x + 3 == width && amount_of_three_deckers > 0) {
      SetHorizontalThreeDecker(x, y);
    }
    if (x + 2 == width && amount_of_two_deckers > 0) {
      SetHorizontalTwoDecker(x, y);
    }
    if (x + 1 == width && amount_of_one_deckers > 0) {
      SetOneDecker( x, y);
    }
    bool one = amount_of_one_deckers;
    bool two = amount_of_two_deckers;
    bool three = amount_of_three_deckers;

    if (!CheckHorizontalShips(x, y) || !(one && two && three)) {
      y = y + 2;
      x = 0;
    }
  }
}

void Board::PlaceHorizontalThreeDeckers(uint64_t& x, uint64_t& y) {
  while (amount_of_three_deckers > 0) {
    if (x + 3 <= width) {
      SetHorizontalThreeDecker(x, y);
    }
    if (x + 2 == width && amount_of_two_deckers > 0) {
      SetHorizontalTwoDecker(x, y);
    }
    if (x + 1 == width && amount_of_one_deckers > 0) {
      SetOneDecker(x, y);
    }
    bool one = amount_of_one_deckers;
    bool two = amount_of_two_deckers;

    if (!CheckHorizontalShips(x, y) || !(one && two)) {
      y = y + 2;
      x = 0;
    }
  }
}

void Board::PlaceHorizontalTwoDeckers(uint64_t& x, uint64_t& y) {
  while (amount_of_two_deckers > 0) {
    if (x + 2 <= width) {
      SetHorizontalTwoDecker(x, y);
    }
    if (x + 1 == width && amount_of_one_deckers > 0) {
      SetOneDecker(x, y);
    }
    bool one = amount_of_one_deckers;

    if (!CheckHorizontalShips(x, y) || !one) {
      y = y + 2;
      x = 0;
    }
  }
}

void Board::PlaceHorizontalOneDeckers(uint64_t& x, uint64_t& y) {
  while (amount_of_one_deckers > 0) {
    if (x + 1 <= width) {
      SetOneDecker(x, y);
      x = x + 2;
    }

    if (!CheckHorizontalShips(x, y)) {
      y = y + 2;
      x = 0;
    }
  }
}

void Board::SetHorizontalFourDecker(uint64_t& x, uint64_t& y) {
  --amount_of_four_deckers;
  Ship* ship = new Ship();
  ship->condition = 4;
  ship->length = 4;
  ship->direction = 'h';
  ship->x = x;
  ship->y = y;
  all_ships.emplace_back(ship);
  for (int i = 0; i < 4; ++i) {
    std::pair<uint64_t, uint64_t> p(x, y);
    my_coordinates[p] = ship;
    ++x;
  }
  ++x;
}

void Board::SetHorizontalThreeDecker(uint64_t& x, uint64_t& y) {
  --amount_of_three_deckers;
  Ship* ship = new Ship();
  ship->condition = 3;
  ship->length = 3;
  ship->direction = 'h';
  ship->x = x;
  ship->y = y;
  all_ships.emplace_back(ship);
  for (int i = 0; i < 3; ++i) {
    std::pair<uint64_t, uint64_t> p(x, y);
    my_coordinates[p] = ship;
    ++x;
  }
  ++x;
}

void Board::SetHorizontalTwoDecker(uint64_t& x, uint64_t& y) {
  --amount_of_two_deckers;
  Ship* ship = new Ship();
  ship->condition = 2;
  ship->length = 2;
  ship->direction = 'h';
  ship->x = x;
  ship->y = y;
  all_ships.emplace_back(ship);
  for (int i = 0; i < 2; ++i) {
    std::pair<uint64_t, uint64_t> p(x, y);
    my_coordinates[p] = ship;
    ++x;
  }
  ++x;
}

bool Board::CheckHorizontalArrangement(uint64_t& x, uint64_t& y, uint8_t& length) {
  uint64_t arr_x = x;
  uint64_t arr_y = y;
  if (arr_y == height - 2) {
    return false;
  }
  --x;
  std::pair<uint64_t, uint64_t> p(x, arr_y);
  if (my_coordinates.count(p)) {
    return false;
  }
  ++arr_y;
  for (int i = 0; i < length + 2; ++i) {
    std::pair<uint64_t, uint64_t> p(x, arr_y);
    if (my_coordinates.count(p)) {
      return false;
    }
    ++x;
  }
  --x;
  --arr_y;
  p = std::make_pair(x, arr_y);
  if (my_coordinates.count(p)) {
    return false;
  }
  --arr_y;
  for (int i = 0; i < length + 2; ++i) {
    std::pair<uint64_t, uint64_t> p(x, arr_y);
    if (my_coordinates.count(p)) {
      return false;
    }
    --x;
  }
  return true;
}