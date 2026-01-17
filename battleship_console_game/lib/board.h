#include <iostream>
#include <map>
#include <utility>
#include <string>

#pragma once

struct Ship {
  short condition;
  short length;
  char direction;
  uint64_t x;
  uint64_t y;
};

struct EnemyShip {
  short length;
  bool is_first_empty = true;
  bool is_last_empty = true;
  std::pair<uint64_t, uint64_t> first_cell;
  std::pair<uint64_t, uint64_t> last_cell;
};

class Board {
 public:
  uint64_t width = 0;
  uint64_t height = 0;
  uint64_t amount_of_one_deckers = 0;
  uint64_t amount_of_two_deckers = 0;
  uint64_t amount_of_three_deckers = 0;
  uint64_t amount_of_four_deckers = 0;
  std::vector<Ship*> all_ships;
  std::map<std::pair<uint64_t, uint64_t>, Ship*> my_coordinates;
  void CleanShips();

  bool CheckDensity(uint64_t& ship_area);
  bool CheckVerticalArrangement(uint64_t& x, uint64_t& y, uint8_t& length);
  bool CheckHorizontalArrangement(uint64_t& x, uint64_t& y, uint8_t& length);

  void PlaceOneDeckersRandom(uint64_t& x, uint64_t& y);

  void PlaceVerticalFourDeckers(uint64_t& x, uint64_t& y);
  void PlaceVerticalThreeDeckers(uint64_t& x, uint64_t& y);
  void PlaceVerticalTwoDeckers(uint64_t& x, uint64_t& y);
  void PlaceVerticalOneDeckers(uint64_t& x, uint64_t& y);

  void PlaceHorizontalFourDeckers(uint64_t& x, uint64_t& y);
  void PlaceHorizontalThreeDeckers(uint64_t& x, uint64_t& y);
  void PlaceHorizontalTwoDeckers(uint64_t& x, uint64_t& y);
  void PlaceHorizontalOneDeckers(uint64_t& x, uint64_t& y);
private:
  bool CheckVerticalShips(uint64_t& x, uint64_t& y);
  bool CheckHorizontalShips(uint64_t& x, uint64_t& y);

  void SetOneDecker(uint64_t& x, uint64_t& y);

  void SetVerticalFourDecker(uint64_t& x, uint64_t& y);
  void SetVerticalThreeDecker(uint64_t& x, uint64_t& y);
  void SetVerticalTwoDecker(uint64_t& x, uint64_t& y);

  void SetHorizontalFourDecker(uint64_t& x, uint64_t& y);
  void SetHorizontalThreeDecker(uint64_t& x, uint64_t& y);
  void SetHorizontalTwoDecker(uint64_t& x, uint64_t& y);
};

