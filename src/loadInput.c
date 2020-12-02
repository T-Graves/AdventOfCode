#include "main.h"

/**
 * Handle freeing all memory being used by input struct.
 */
void freeInput(Input* input) {
  // Free error message memory.
  free(input->errMessage);

  // Free all input line memory.
  for (int i = 0; i < input->lineCount; i++) {
    free(input->inputLines[i]);
  }

  // Free input line string array mem.
  free(input->inputLines);
  // Free file path mem.
  free(input->inputFilePath);
}

/**
 * Handle loading, parsing, and storing a file and its values into a struct.
 */
void loadInput(char* inputFilePath, Input* input) {
  // Store input file path in input struct. No idea if I will actually need it,
  // but doesn't hurt to have it around, so whatever.
  input->inputFilePath = malloc(strlen(inputFilePath) + 1);
  strcpy(input->inputFilePath, inputFilePath);

  // Open handle to input file in read only mode.
  FILE* fHandle = fopen(inputFilePath, "r");

  // Make sure the handle opened properly.
  if (fHandle == NULL) {
    input->errCode = 1;
    char* errMessage = "Error opening file";
    input->errMessage = malloc(strlen(errMessage) + 1);
    strcpy(input->errMessage, errMessage);
    return;
  }

  // Figure out total lines and longest line length.
  int lineCount = 0;
  int maxLineLength = 0;
  int currLineLength = 0;
  char c;
  do {
    // Get a character.
    c = fgetc(fHandle);
    // Check if it's the end of the line or file.
    if (c == '\n' || c == '\r' || c == EOF) {

      // increment line counter.
      lineCount++;

      // Check if this current line had a longer length than the current max.
      if (maxLineLength <= currLineLength) {
        maxLineLength = currLineLength;
      }
      // Reset current line length count.
      currLineLength = 0;
    }
    else {
      // Increment current line length count.
      currLineLength++;
    }
  } while (c != EOF);

  // Reset file handle to beginning of file.
  rewind(fHandle);

  // Store the max line length and total line count.
  input->lineLength = maxLineLength;
  input->lineCount = lineCount;

  // Get memory to store the input.
  input->inputLines = malloc(sizeof(char*) * input->lineCount);

  // Set up intermediate array for fgets to put the current line into.
  char line[maxLineLength + 2];

  // Read each line of the file and store it into the inputLines array.
  int i = 0;
  while (fgets(line, maxLineLength + 2, fHandle) != NULL) {
    input->inputLines[i] = malloc(maxLineLength + 2);
    strcpy(input->inputLines[i], line);
    i++;
  }

  // Set error code to 0 (success I guess).
  input->errCode = 0;
  // calloc to make sure this isn't pointing to junk data.
  input->errMessage = calloc(sizeof(char*), 1);

  // Close file handle.
  fclose(fHandle);
}
