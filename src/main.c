#include "main.h"

/**
 * Advent of Code 2020.
 * Using advent of code as a reason to finally get back into learning C.
 */

int main(int argc, char** argv) {

  // Argument 2 should be which puzzle to run.
  if (argc < 2) {
    perror("Requires first argument to be which puzzle to run: day01 - day25");
    return 1;
  }

  // Loop over puzzles list until we find the one requested or we hit the end.
  for (const Puzzle *puzzle = puzzles; puzzle->puzzleId != 0; puzzle++) {
    // Check if this is the requested puzzle.
    if (strcmp(puzzle->puzzleId, argv[1]) == 0) {
      // This is the correct puzzle so load input and run puzzle function.
      Input input;
      loadInput(puzzle->inputFilePath, &input);
      (*puzzle->func)(input);

      // Free input memory.
      freeInput(&input);

      // Exit loop early since we found the puzzle we wanted to run.
      break;
    }
	}

  return 0;
}

bool stol(char *str, long *val) {
  errno = 0;
  char *endptr;
  *val = strtol(str, &endptr, 10);

  if (errno == ERANGE || endptr == str) {
    return false;
  }
  else {
    return true;
  }
}
