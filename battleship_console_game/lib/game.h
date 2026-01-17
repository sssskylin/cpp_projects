#include <fstream>
#include <cstdlib>
#include <vector>
#include "board.h"
#include "strategy.h"

#pragma once

class Game {
 public:
  std::string path_to_save_ships = "";
  std::string path_to_download_ships = "";
  std::string game_mode = "";
  std::string strategy = "custom";

  bool is_started = false;
  bool is_stopped = false;
  bool is_finished = false;
  bool is_winner = false;
  bool is_loser = false;

  uint64_t last_x = 0;
  uint64_t last_y = 0;
  std::string last_result;

  Board new_board;
  EnemyShip enemy_ship;
  Ordered ordered_strategy;
  Custom custom_strategy;

  uint64_t ship_area;
  std::set<std::pair<uint64_t, uint64_t>> shot_cells;

  void CreateRules();
  void SetShips();
  void Shot();
  void SetResult(uint64_t& x, uint64_t& y);
  void GetShot(uint64_t& x, uint64_t& y);
  void DumpShips(std::string& path_to_save_ships);
  
 private:
  void CountShipArea();
  void FillEnemyShip(std::pair<uint64_t, uint64_t>& p);
  void VerticalTraversal(uint64_t& x, uint64_t& y, short& length);
  void HorizontalTraversal(uint64_t& x, uint64_t& y, short& length);
};