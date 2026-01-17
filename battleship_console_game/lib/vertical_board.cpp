#include "board.h"
#include <fstream>
#include <iostream>
#include <string>
#include <utility>

bool Board::CheckVerticalShips(uint64_t& x, uint64_t& y) {
  if (y >= height) {
    return false;
  }
  return true;
}

void Board::PlaceVerticalFourDeckers(uint64_t& x, uint64_t& y) {
  while (amount_of_four_deckers > 0) {
    if (y + 4 <= height) {
      SetVerticalFourDecker(x, y);
    }
    if (y + 3 == height && amount_of_three_deckers > 0) {
      SetVerticalThreeDecker(x, y);
    }
    if (y + 2 == height && amount_of_two_deckers > 0) {
      SetVerticalTwoDecker(x, y);
    }
    if (y + 1 == height && amount_of_one_deckers > 0) {
      SetOneDecker(x, y);
    }
    bool one = amount_of_one_deckers;
    bool two = amount_of_two_deckers;
    bool three = amount_of_three_deckers;

    if (!CheckVerticalShips(x, y) || !(one && two && three)) {
      x = x + 2;
      y = 0;
    }
  }
}

void Board::PlaceVerticalThreeDeckers(uint64_t& x, uint64_t& y) {
  while (amount_of_three_deckers > 0) {
    if (y + 3 <= height) {
      SetVerticalThreeDecker(x, y);
    }
    if (y + 2 == height && amount_of_two_deckers > 0) {
      SetVerticalTwoDecker(x, y);
    }
    if (y + 1 == height && amount_of_one_deckers > 0) {
      SetOneDecker( x, y);
    }
    bool one = amount_of_one_deckers;
    bool two = amount_of_two_deckers;

    if (!CheckVerticalShips(x, y) || !(one && two)) {
      x = x + 2;
      y = 0;
    }
  }
}

void Board::PlaceVerticalTwoDeckers(uint64_t& x, uint64_t& y) {
  while (amount_of_two_deckers > 0) {
    if (y + 2 <= height) {
      SetVerticalTwoDecker(x, y);
    }
    if (y + 1 == height && amount_of_one_deckers > 0) {
      SetOneDecker(x, y);
    }
    bool one = amount_of_one_deckers;

    if (!CheckVerticalShips(x, y) || !one) {
      x = x + 2;
      y = 0;
    }
  }
}

void Board::PlaceVerticalOneDeckers(uint64_t& x, uint64_t& y) {
  while (amount_of_one_deckers > 0) {
    if (y + 1 <= height) {
      SetOneDecker(x, y);
      y = y + 2;
    }

    if (!CheckVerticalShips(x, y)) {
      x = x + 2;
      y = 0;
    }
  }
}

void Board::SetVerticalFourDecker(uint64_t& x, uint64_t& y) {
  --amount_of_four_deckers;
  Ship* ship = new Ship();
  ship->condition = 4;
  ship->length = 4;
  ship->direction = 'v';
  ship->x = x;
  ship->y = y;
  all_ships.emplace_back(ship);
  for (int i = 0; i < 4; ++i) {
    std::pair<uint64_t, uint64_t> p(x, y);
    my_coordinates[p] = ship;
    ++y;
  }
  ++y;
}

void Board::SetVerticalThreeDecker(uint64_t& x, uint64_t& y) {
  --amount_of_three_deckers;
  Ship* ship = new Ship();
  ship->condition = 3;
  ship->length = 3;
  ship->direction = 'v';
  ship->x = x;
  ship->y = y;
  all_ships.emplace_back(ship);
  for (int i = 0; i < 3; ++i) {
    std::pair<uint64_t, uint64_t> p(x, y);
    my_coordinates[p] = ship;
    ++y;
  }
  ++y;
}

void Board::SetVerticalTwoDecker(uint64_t& x, uint64_t& y) {
  --amount_of_two_deckers;
  Ship* ship = new Ship();
  ship->condition = 2;
  ship->length = 2;
  ship->direction = 'v';
  ship->x = x;
  ship->y = y;
  all_ships.emplace_back(ship);
  for (int i = 0; i < 2; ++i) {
    std::pair<uint64_t, uint64_t> p(x, y);
    my_coordinates[p] = ship;
    ++y;
  }
  ++y;
}

bool Board::CheckVerticalArrangement(uint64_t& x, uint64_t& y, uint8_t& length) {
  uint64_t arr_x = x;
  uint64_t arr_y = y;
  if (arr_x == width - 2) {
    return false;
  }
  --arr_y;
  std::pair<uint64_t, uint64_t> p(x, y);
  if (my_coordinates.count(p)) {
    return false;
  }
  ++arr_x;
  for (int i = 0; i < length + 2; ++i) {
    std::pair<uint64_t, uint64_t> p(x, arr_y);
    if (my_coordinates.count(p)) {
      return false;
    }
    ++arr_y;
  }
  --arr_y;
  --arr_x;
  p = std::make_pair(x, arr_y);
  if (my_coordinates.count(p)) {
    return false;
  }
  --arr_x;
  for (int i = 0; i < length + 2; ++i) {
    std::pair<uint64_t, uint64_t> p(x, arr_y);
    if (my_coordinates.count(p)) {
      return false;
    }
    --arr_y;
  }
  return true;
}