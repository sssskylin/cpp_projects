#include "sandpile.h"
#include "bmp.h"
#include "conversion.h"

const int kMaxSizeOfGrid = 32768;
const int kInitialSizeOfGrid = 2;
const int kMinBorder = 0;
const int kMaxBorder = 0;

void Sandpile::CreateSandpile(char* input_file) {
    min_x = kMaxSizeOfGrid; 
    min_y = kMaxSizeOfGrid; 
    
    len_x = kInitialSizeOfGrid; 
    len_y = kInitialSizeOfGrid; 

    upper_border = kMaxBorder;
    lower_border = kMinBorder;
    right_border = kMinBorder;
    left_border = kMaxBorder;

    grid = new long long*[len_y];
    for (long long i = 0; i < len_y; ++i) {
        grid[i] = new long long[len_x]();
    }

    std::ifstream infile;
    infile.open(input_file);
    long long x, y, count;
    while (infile >> x >> y >> count) {
        if (x < min_x) {
            min_x = x;
        }
        if (y < min_y) {
            min_y = y;
        }
    }
    infile.close();
    std::ifstream file;
    file.open(input_file);
    long long x_res, y_res, count_res;
    while (file >> x_res >> y_res >> count_res) {

        long long grid_x = x_res - min_x; 
        long long grid_y = y_res - min_y;

        while (grid_x >= len_x || grid_y >= len_y) {
            ExpandGrid();
        }
        
        grid[grid_y][grid_x] = count_res;
    }
}

void Sandpile::Simulate(int max_iter, int freq, char* output_directory) {
  int iter = 0;
  while (!IsStable()) {
    if (iter == max_iter) {
      break;
    }
    UpdateGrid();
    if (freq > 0 && iter % freq == 0) {
      SaveState(iter, output_directory);
    }
    ++iter;
  }
  SaveState(iter, output_directory);
}

void Sandpile::UpdateGrid() {
  long long** new_grid = grid;
  bool expanded = false;
  len_black_list = 1;
  black_list = new Pair[len_black_list];
  long long bl = 0;
  for (long long i = 0; i < len_y; ++i) {
    for (long long j = 0; j < len_x; ++j) {
      long long x = j;
      long long y = i;
      long long count = grid[i][j];

      if (count > 3 && (x == 0 || x == len_x - 1 || y == 0 || y == len_y - 1) &&
          expanded == false) {
        ExpandGrid();
        expanded = true;
      }
    }
  }
  for (long long i = 0; i < len_y; ++i) {
    for (long long j = 0; j < len_x; ++j) {
      long long x = j;
      long long y = i;
      long long count = grid[i][j];
      if (count > 3) {
        if (bl == len_black_list) {
          ExpandList();
        }
        black_list[bl].coord_y = y;
        black_list[bl].coord_x = x;
        ++bl;
      }
    }
  }
  bool upper = false;
  bool left = false;
  for (long long i = 0; i < len_black_list; ++i) {
    long long x = black_list[i].coord_x;
    long long y = black_list[i].coord_y;
    if (x > -1 && y > -1) {
      long long count = grid[y][x];

      count -= 4;
      grid[y][x] = count;
      grid[y][x + 1] += 1;
      grid[y][x - 1] += 1;
      grid[y + 1][x] += 1;
      grid[y - 1][x] += 1;
      if (upper_border > y - 1) {
        upper_border = y - 1;
      }
      if (lower_border < y + 1) {
        lower_border = y + 1;
      }
      if (right_border < x + 1) {
        right_border = x + 1;
      }
      if (left_border > x - 1) {
        left_border = x - 1;
      }
    }
  }
  delete[] black_list;
}

bool Sandpile::IsStable() {
  for (long long i = 0; i < len_y; i++) {
    for (long long j = 0; j < len_x; j++) {
      if (grid[i][j] > 3) return false;
    }
  }
  return true;
}

void Sandpile::ExpandGrid() {
  long long new_len_x = len_x * 2;
  long long new_len_y = len_y * 2;
  long long** new_grid = new long long*[new_len_y];
  for (long long i = 0; i < new_len_y; ++i) {
    new_grid[i] = new long long[new_len_x];
    for (long long j = 0; j < new_len_x; ++j) {
      new_grid[i][j] = 0;
    }
  }
  for (long long i = 0; i < len_y; i++) {
    for (long long j = 0; j < len_x; j++) {
      new_grid[i + len_y / 2][j + len_x / 2] = grid[i][j];
    }
  }
  delete[] grid;
  grid = new long long*[new_len_y];
  for (int i = 0; i < new_len_y; i++) {
    grid[i] = new long long[new_len_x];
    grid[i] = new_grid[i];
  }
  delete[] new_grid;
  upper_border=len_y/2;
  left_border=len_x/2;
  len_x = new_len_x;
  len_y = new_len_y;
}

void Sandpile::SaveState(int iteration, char* output_directory) {
  long long** image_data = new long long*[len_y];
  for (long long i = 0; i < len_y; ++i) {
    image_data[i] = new long long[len_x];
    for (long long j = 0; j < len_x; ++j) {
      image_data[i][j] = grid[i][j];
    }
  }
  char* filename = AddChar(output_directory, "/sandpile_");
  filename = AddChar(filename, Uint64_tToChar(iteration));
  filename = AddChar(filename, ".bmp");

  int width = right_border - left_border + 1;
  int height = lower_border - upper_border + 1;

  createBMP(filename, width, height, image_data, upper_border, left_border);
  delete[] image_data;
}

void Sandpile::ExpandList() {
  long long new_len_black_list = len_black_list * 2;
  Pair* new_black_list = new Pair[new_len_black_list];
  for (long long k = 0; k < new_len_black_list; ++k) {
    new_black_list[k].coord_y = -1;
    new_black_list[k].coord_x = -1;
  }
  for (long long k = 0; k < len_black_list; ++k) {
    new_black_list[k] = black_list[k];
  }
  delete[] black_list;
  black_list = new Pair[new_len_black_list];
  for (int i = 0; i < new_len_black_list; i++) {
    black_list[i] = new_black_list[i];
  }
  len_black_list = new_len_black_list;
  delete[] new_black_list;
}