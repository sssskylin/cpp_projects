#include <iostream>
#include "../lib/conversion.h"
#include "../lib/sandpile.h"

int main(int argc, char* argv[]) {
  char* input_file;
  char* output_directory;
  int max_iter = 10000000;
  int freq = 0;

  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input") == 0) {
      input_file = argv[i+1];
    } else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
      output_directory = argv[i+1];
    } else if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--max-iter") == 0) {
      max_iter = CharToUint64_t(argv[++i]);
    } else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--freq") == 0) {
      freq = CharToUint64_t(argv[++i]);
    }
  }

  if (input_file == "" || output_directory == "") {
    exit(1);
  }

  Sandpile sandpile;

  sandpile.CreateSandpile(input_file);

  sandpile.Simulate(max_iter, freq, output_directory);

}
