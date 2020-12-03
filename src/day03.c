#include <stdint.h>
#include "main.h"

/**
 * --- Day 3: Toboggan Trajectory ---
 * With the toboggan login problems resolved, you set off toward the airport.
 * While travel by toboggan might be easy, it's certainly not safe: there's
 * very minimal steering and the area is covered in trees. You'll need to see
 * which angles will take you near the fewest trees.
 *
 * Due to the local geology, trees in this area only grow on exact integer
 * coordinates in a grid. You make a map (your puzzle input) of the open
 * squares (.) and trees (#) you can see. For example:
 * ..##.......
 * #...#...#..
 * .#....#..#.
 * ..#.#...#.#
 * .#...##..#.
 * ..#.##.....
 * .#.#.#....#
 * .#........#
 * #.##...#...
 * #...##....#
 * .#..#...#.#
 *
 * These aren't the only trees, though; due to something you read about once
 * involving arboreal genetics and biome stability, the same pattern repeats
 * to the right many times:
 * ..##.........##.........##.........##.........##.........##.......  --->
 * #...#...#..#...#...#..#...#...#..#...#...#..#...#...#..#...#...#..
 * .#....#..#..#....#..#..#....#..#..#....#..#..#....#..#..#....#..#.
 * ..#.#...#.#..#.#...#.#..#.#...#.#..#.#...#.#..#.#...#.#..#.#...#.#
 * .#...##..#..#...##..#..#...##..#..#...##..#..#...##..#..#...##..#.
 * ..#.##.......#.##.......#.##.......#.##.......#.##.......#.##.....  --->
 * .#.#.#....#.#.#.#....#.#.#.#....#.#.#.#....#.#.#.#....#.#.#.#....#
 * .#........#.#........#.#........#.#........#.#........#.#........#
 * #.##...#...#.##...#...#.##...#...#.##...#...#.##...#...#.##...#...
 * #...##....##...##....##...##....##...##....##...##....##...##....#
 * .#..#...#.#.#..#...#.#.#..#...#.#.#..#...#.#.#..#...#.#.#..#...#.#  --->
 *
 * You start on the open square (.) in the top-left corner and need to reach the
 * bottom (below the bottom-most row on your map).
 *
 * The toboggan can only follow a few specific slopes (you opted for a cheaper
 * model that prefers rational numbers); start by counting all the trees you
 * would encounter for the slope right 3, down 1:
 *
 * From your starting position at the top-left, check the position that is right
 * 3 and down 1. Then, check the position that is right 3 and down 1 from there,
 * and so on until you go past the bottom of the map.
 *
 * The locations you'd check in the above example are marked here with O where
 * there was an open square and X where there was a tree:
 * ..##.........##.........##.........##.........##.........##.......  --->
 * #..O#...#..#...#...#..#...#...#..#...#...#..#...#...#..#...#...#..
 * .#....X..#..#....#..#..#....#..#..#....#..#..#....#..#..#....#..#.
 * ..#.#...#O#..#.#...#.#..#.#...#.#..#.#...#.#..#.#...#.#..#.#...#.#
 * .#...##..#..X...##..#..#...##..#..#...##..#..#...##..#..#...##..#.
 * ..#.##.......#.X#.......#.##.......#.##.......#.##.......#.##.....  --->
 * .#.#.#....#.#.#.#.O..#.#.#.#....#.#.#.#....#.#.#.#....#.#.#.#....#
 * .#........#.#........X.#........#.#........#.#........#.#........#
 * #.##...#...#.##...#...#.X#...#...#.##...#...#.##...#...#.##...#...
 * #...##....##...##....##...#X....##...##....##...##....##...##....#
 * .#..#...#.#.#..#...#.#.#..#...X.#.#..#...#.#.#..#...#.#.#..#...#.#  --->
 *
 * In this example, traversing the map using this slope would cause you to
 * encounter 7 trees.
 *
 * Starting at the top-left corner of your map and following a slope of right 3
 * and down 1, how many trees would you encounter?
 *
 * --- Part Two ---
 * Time to check the rest of the slopes - you need to minimize the probability
 * of a sudden arboreal stop, after all.
 *
 * Determine the number of trees you would encounter if, for each of the
 * following slopes, you start at the top-left corner and traverse the map all
 * the way to the bottom:
 *
 * Right 1, down 1.
 * Right 3, down 1. (This is the slope you already checked.)
 * Right 5, down 1.
 * Right 7, down 1.
 * Right 1, down 2.
 * In the above example, these slopes would find 2, 7, 3, 4, and 2 tree(s)
 * respectively; multiplied together, these produce the answer 336.
 *
 * What do you get if you multiply together the number of trees encountered on
 * each of the listed slopes?
 */

int checkSlope(Input, int, int);

void day03(Input input) {
  // Check single path.
  int puzzleOne = checkSlope(input, 3, 1);
  printf("Part 1: %d\n", puzzleOne);

  // Prepare path list.
  int paths[5][2] = {
    {1, 1},
    {3, 1},
    {5, 1},
    {7, 1},
    {1, 2}
  };

  // Prepare result output. Big AF number.
  uint64_t puzzleTwo = 0;
  // Check each path and multiply it with the puzzle two total.
  for (int i = 0; i < 5; i++) {
    int treesHit = checkSlope(input, paths[i][0], paths[i][1]);
    if (puzzleTwo == 0) {
      puzzleTwo = treesHit;
    }
    else {
      puzzleTwo *= treesHit;
    }
  }
  printf("Part 2: %I64d\n", puzzleTwo);
}

int checkSlope(Input input, int xMov, int yMov) {
  // Starting position.
  int xPos = 0;
  int yPos = 0;

  int treesHit = 0;
  int currLine = 0;
  while (currLine < input.lineCount) {
    // Check if we hit a tree.
    if (input.inputLines[yPos][xPos] == '#') {
      treesHit++;
    }

    // Update x position. If we go past the line length, loop back to the front.
    xPos += xMov;
    if (xPos >= input.lineLength) {
      xPos -= input.lineLength;
    }

    // Update y position. If we go past line count, set to last line
    // (-1 for array)
    yPos += yMov;
    if (yPos >= input.lineCount) {
      yPos = input.lineCount - 1;
    }
    // Increment the current line counter using the yMov value so we can keep
    // track of our actual position on the slope.
    currLine += yMov;
  }

  return treesHit;
}
