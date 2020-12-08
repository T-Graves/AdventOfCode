#include "main.h"

/**
 * --- Day 5: Binary Boarding ---
 * You board your plane only to discover a new problem: you dropped your
 * boarding pass! You aren't sure which seat is yours, and all of the flight
 * attendants are busy with the flood of people that suddenly made it through
 * passport control.
 *
 * You write a quick program to use your phone's camera to scan all of the
 * nearby boarding passes (your puzzle input); perhaps you can find your seat
 * through process of elimination.
 *
 * Instead of zones or groups, this airline uses binary space partitioning to
 * seat people. A seat might be specified like FBFBBFFRLR, where F means
 * "front", B means "back", L means "left", and R means "right".
 *
 * The first 7 characters will either be F or B; these specify exactly one of
 * the 128 rows on the plane (numbered 0 through 127). Each letter tells you
 * which half of a region the given seat is in. Start with the whole list of
 * rows; the first letter indicates whether the seat is in the front
 * (0 through 63) or the back (64 through 127). The next letter indicates which
 * half of that region the seat is in, and so on until you're left with exactly
 * one row.
 *
 * For example, consider just the first seven characters of FBFBBFFRLR:
 *
 * Start by considering the whole range, rows 0 through 127.
 * F means to take the lower half, keeping rows 0 through 63.
 * B means to take the upper half, keeping rows 32 through 63.
 * F means to take the lower half, keeping rows 32 through 47.
 * B means to take the upper half, keeping rows 40 through 47.
 * B keeps rows 44 through 47.
 * F keeps rows 44 through 45.
 * The final F keeps the lower of the two, row 44.
 *
 * The last three characters will be either L or R; these specify exactly one of
 * the 8 columns of seats on the plane (numbered 0 through 7). The same process
 * as above proceeds again, this time with only three steps. L means to keep the
 * lower half, while R means to keep the upper half.
 *
 * For example, consider just the last 3 characters of FBFBBFFRLR:
 *
 * Start by considering the whole range, columns 0 through 7.
 * R means to take the upper half, keeping columns 4 through 7.
 * L means to take the lower half, keeping columns 4 through 5.
 * The final R keeps the upper of the two, column 5.
 *
 * So, decoding FBFBBFFRLR reveals that it is the seat at row 44, column 5.
 *
 * Every seat also has a unique seat ID: multiply the row by 8, then add the
 * column. In this example, the seat has ID 44 * 8 + 5 = 357.
 *
 * Here are some other boarding passes:
 *
 * BFFFBBFRRR: row 70, column 7, seat ID 567.
 * FFFBBBFRRR: row 14, column 7, seat ID 119.
 * BBFFBBFRLL: row 102, column 4, seat ID 820.
 * As a sanity check, look through your list of boarding passes. What is the
 * highest seat ID on a boarding pass?
 *
 * --- Part Two ---
 * Ding! The "fasten seat belt" signs have turned on. Time to find your seat.
 *
 * It's a completely full flight, so your seat should be the only missing
 * boarding pass in your list. However, there's a catch: some of the seats at
 * the very front and back of the plane don't exist on this aircraft, so they'll
 * be missing from your list as well.
 *
 * Your seat wasn't at the very front or back, though; the seats with IDs +1 and
 * -1 from yours will be in your list.
 *
 * What is the ID of your seat?
 */

// 0 - 127.
const int PLAIN_SEAT_ROWS = 127;
// 0 - 7.
const int PLAIN_SEAT_COLUMNS = 7;

// F = front (lower half).
// B = back (upper half).
// L = left (lower half).
// R = right (upper half).

// Row    Col
// |-----||-|
// BBFFBFBRLL
// B - 64-127
// B - 96-127
// F - 96-111
// F - 96-103
// B - 100-103
// F - 100-101
// B - 101

void day05(Input input) {
  int highestId = 0;
  int seatList[1000] = {0};

  for (int i = 0; i < input.lineCount; i++) {
    // Make sure we don't try to parse null or blank lines.
    if (input.inputLines[i] == NULL
      || input.inputLines[i][0] == ' '
      || input.inputLines[i][0] == '\n'
      || input.inputLines[i][0] == '\r') {
      continue;
    }

    // Prepare upper and lower bounds.
    float rowUpper = PLAIN_SEAT_ROWS;
    float rowLower = 0;
    // Prepare final row storage.
    int row = 0;

    // Flag to check whether we should take the upper bound or lower bound as
    // our final value.
    bool takeRowUpper = false;
    // Loop over first 7 characters to find the row.
    for (int j = 0; j < 7; j++) {
      if (input.inputLines[i][j] == 'F') {
        takeRowUpper = false;
        rowUpper -= ceilf((rowUpper - rowLower) / 2.0f);
      }
      if (input.inputLines[i][j] == 'B') {
        takeRowUpper = true;
        rowLower += ceilf((rowUpper - rowLower) / 2.0f);
      }
    }

    // Check if we need to use upper or lower bound.
    if (takeRowUpper) {
      row = rowUpper;
    }
    else {
      row = rowLower;
    }

    // Prepare upper and lower bounds.
    float colUpper = PLAIN_SEAT_COLUMNS;
    float colLower = 0;
    // Prepare final col storage.
    int col = 0;

    // Flag to check whether we should take the upper bound or lower bound as
    // our final value.
    bool takeColUpper = false;
    // Loop over last 3 characters to find the column.
    for (int j = 7; j < 10; j++) {
      if (input.inputLines[i][j] == 'L') {
        takeColUpper = false;
        colUpper -= ceilf((colUpper - colLower) / 2.0f);
      }
      if (input.inputLines[i][j] == 'R') {
        takeColUpper = true;
        colLower += ceilf((colUpper - colLower) / 2.0f);
      }
    }

    // Check if we need to use upper or lower bound.
    if (takeColUpper) {
      col = colUpper;
    }
    else {
      col = colLower;
    }

    // Generate the unique seat id.
    int seatId = (row * 8) + col;
    // Add seat id to seat list.
    seatList[seatId] = 1;
    // Check if this is the highest seat id.
    if (seatId > highestId) {
      highestId = seatId;
    }
  }

  printf("Part 1: %d\n", highestId);

  // Loop over seat list until we reach the highest seat id.
  for (int i = 0; i <= highestId; i++) {
    if (seatList[i] == 0) {
      // We reached the highest seat id and didn't find our seat.
      // Don't bother to read past this point.
      if (i + 1 > highestId) {
        printf("Part 2: Seat not found\n");
        break;
      }
      if (seatList[i - 1] == 1 && seatList[i + 1] == 1 ) {
        printf("Part 2: %d\n", i);
        // Found our seat. Don't need to process anymore.
        break;
      }
    }
  }
}
