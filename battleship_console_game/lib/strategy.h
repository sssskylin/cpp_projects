#include "board.h"
#include <set>

#pragma once

class Strategy {
 public:
  uint64_t x = 0;
  uint64_t y = 0;
};

class Ordered : public Strategy {
 public:
  void Change(uint64_t& width);
};

class Custom : public Strategy {
 public:
  uint64_t SetX();
  uint8_t max_len = 1;
  uint8_t hit_counter = 0;
  std::string last_dir = "";
  bool killer_mode = false;
  void Change(Board& new_board, std::string& last_result, std::set<std::pair<uint64_t, uint64_t>>& shot_cells);

 private:
  bool is_start = 0;
  uint64_t usual_x;
  uint64_t usual_y;
  uint64_t counter = 1;
  bool is_end = 0;
  bool CheckRight(Board& new_board);
  bool CheckUp(Board& new_board);
  bool CheckLeft(Board& new_board);
  bool CheckDown(Board& new_board);
};