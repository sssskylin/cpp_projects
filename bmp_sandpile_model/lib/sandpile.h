#ifndef SANDPILE_H
#define SANDPILE_H

#include <iostream>
#include <fstream>

struct Pair{
    long long coord_x;
    long long coord_y;
    };

struct Sandpile {
    long long** grid;
    long long len_x;
    long long len_y;
    int min_len;
    int min_x;
    int min_y;
    int upper_border;
    int lower_border;
    int right_border;
    int left_border;
    Pair* black_list;
    long long len_black_list;
    void CreateSandpile(char* input_file);
    void Simulate(int max_iter, int freq, char* output_directory);
    void SaveState(int iteration, char* output_directory);

    void UpdateGrid();
    bool IsStable();
    void ExpandGrid();
    void ExpandList();
};

#endif