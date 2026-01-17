#include "strategy.h"
#include "board.h"

void Ordered::Change(uint64_t& width) {
  if (x < width - 1) {
    ++x;
  } else {
    x = 0;
    ++y;
  }
}

uint64_t Custom::SetX() {
  x = max_len - 1;
  return x;
}

void Custom::Change(Board& new_board, std::string& last_result,
                    std::set<std::pair<uint64_t, uint64_t>>& shot_cells) {
  if (is_start == 0) { 
    SetX();
    is_start = 1;
  }
  if (!killer_mode) {
    std::pair<uint64_t, uint64_t> p(x, y);
    while (shot_cells.count(p)) {
      if ((x == 0 || y == new_board.height - 1) && is_end == 0) {
        ++counter;
        x = counter * max_len - 1;
        y = 0;
        if (x >= new_board.width) {
          y = x - new_board.width + 1;
          x = new_board.width - 1;
        }
        if (y >= new_board.height && is_end == 0) {
          is_end = 1;
          x = std::rand() % new_board.width;
          y = std::rand() % new_board.height;
        }
      } else if (is_end == 0) {
        --x;
        ++y;
      } else {
        x = std::rand() % new_board.width;
        y = std::rand() % new_board.height;
      }
      p = std::make_pair(x, y);
    }
    shot_cells.insert(p);
  } else {
    if (last_dir == "") {
      usual_x = x;
      usual_y = y;
      if (CheckRight(new_board)) {
        ++x;
        last_dir = "right";
      } else if (CheckUp(new_board)) {
        --y;
        last_dir = "up";
      } else if (CheckLeft(new_board)) {
        --x;
        last_dir = "left";
      } else {
        ++y;
        last_dir = "down";
      }
    } else if (last_dir == "right") {
      if (last_result == "miss") {
        if (hit_counter) {
          x = usual_x - 1;
          last_dir = "left";
        } else {
          if (CheckUp(new_board)) {
            --x;
            --y;
            last_dir = "up";
          } else if (CheckLeft(new_board)) {
            x = x - 2;
            last_dir = "left";
          } else {
            --x;
            ++y;
            last_dir = "down";
          }
        }
      } else if (last_result == "hit") {
        ++x;
      }
    } else if (last_dir == "up") {
      if (last_result == "miss") {
        if (hit_counter) {
          y = usual_y + 1;
          last_dir = "down";
        } else {
          if (CheckLeft(new_board)) {
            --x;
            ++y;
            last_dir = "left";
          } else {
            y = y + 2;
            last_dir = "down";
          }
        }
      } else if (last_result == "hit") {
        --y;
      }
    } else if (last_dir == "left") {
      if (last_result == "miss") {
        if (hit_counter) {
          x = usual_x + 1;
          last_dir = "right";
        } else {
          ++x;
          ++y;
          last_dir = "down";
        }
      } else if (last_result == "hit") {
        --x;
      }
    } else if (last_dir == "down") {
      if (last_result == "miss") {
        y = usual_y - 1;
        last_dir = "up";
      } else if (last_result == "hit") {
        ++y;
      }
    }
  }
  std::pair<uint64_t, uint64_t> p(x, y);
  shot_cells.insert(p);
}

bool Custom::CheckRight(Board& new_board) {
  if (x + 1 == new_board.width) {
    return false;
  }
  return true;
}

bool Custom::CheckUp(Board& new_board) {
  if (y == 0) {
    return false;
  }
  return true;
}

bool Custom::CheckLeft(Board& new_board) {
  if (x == 0) {
    return false;
  }
  return true;
}

bool Custom::CheckDown(Board& new_board) {
  if (y + 1 == new_board.height) {
    return false;
  }
  return true;
}