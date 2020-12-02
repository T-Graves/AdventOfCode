#ifndef _MAIN_H
#define _MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Input_t Input;
typedef struct Puzzle_t Puzzle;

struct Input_t {
  char* inputFilePath;
  char** inputLines;
  int lineCount;
  int lineLength;

  int errCode;
  char* errMessage;
};

struct Puzzle_t {
  char* puzzleId;
  void (*func)(Input);
  char* inputFilePath;
};

void freeInput(Input*);
void loadInput(char*, Input*);

void day01(Input);
void day02(Input);
void day03(Input);
void day04(Input);
void day05(Input);
void day06(Input);
void day07(Input);
void day08(Input);
void day09(Input);
void day10(Input);
void day11(Input);
void day12(Input);
void day13(Input);
void day14(Input);
void day15(Input);
void day16(Input);
void day17(Input);
void day18(Input);
void day19(Input);
void day20(Input);
void day21(Input);
void day22(Input);
void day23(Input);
void day24(Input);
void day25(Input);

static const Puzzle puzzles[] = {
  {"day01", day01, "../assets/inputDay01.txt"},
  {"day02", day02, "../assets/inputDay02.txt"},
  {"day03", day03, "../assets/inputDay03.txt"},
  {"day04", day04, "../assets/inputDay04.txt"},
  {"day05", day05, "../assets/inputDay05.txt"},
  {"day06", day06, "../assets/inputDay06.txt"},
  {"day07", day07, "../assets/inputDay07.txt"},
  {"day08", day08, "../assets/inputDay08.txt"},
  {"day09", day09, "../assets/inputDay09.txt"},
  {"day10", day10, "../assets/inputDay10.txt"},
  {"day11", day11, "../assets/inputDay11.txt"},
  {"day12", day12, "../assets/inputDay12.txt"},
  {"day13", day13, "../assets/inputDay13.txt"},
  {"day14", day14, "../assets/inputDay14.txt"},
  {"day15", day15, "../assets/inputDay15.txt"},
  {"day16", day16, "../assets/inputDay16.txt"},
  {"day17", day17, "../assets/inputDay17.txt"},
  {"day18", day18, "../assets/inputDay18.txt"},
  {"day19", day19, "../assets/inputDay19.txt"},
  {"day20", day20, "../assets/inputDay20.txt"},
  {"day21", day21, "../assets/inputDay21.txt"},
  {"day22", day22, "../assets/inputDay22.txt"},
  {"day23", day23, "../assets/inputDay23.txt"},
  {"day24", day24, "../assets/inputDay24.txt"},
  {"day25", day25, "../assets/inputDay25.txt"},
  {0, 0, 0}
};

#endif
