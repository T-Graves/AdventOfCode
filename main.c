#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Input_t {
  char* inputFilePath;
  char** inputLines;
  int lineCount;
  int lineLength;

  int errCode;
  char* errMessage;
} Input;

void loadInput(char*, Input*);
void freeInput(Input*);

int main(int argc, char** argv) {

  Input input;
  loadInput("Day_01/input.txt", &input);

  // @ToDo: Need to do error checking to make sure we loaded the file
  //        and stored its values before we attempt to loop over them.

  int outOne = 0;
  for (int i = 0; i < input.lineCount; i++) {
    int a = atoi(input.inputLines[i]);
    for (int j = 0; j < input.lineCount; j++) {
      int b = atoi(input.inputLines[j]);
      if (a + b == 2020) {
        outOne = a * b;
      }
    }
  }

  printf("%d\n", outOne);

  int outTwo = 0;
  for (int i = 0; i < input.lineCount; i++) {
    int a = atoi(input.inputLines[i]);
    for (int j = 0; j < input.lineCount; j++) {
      int b = atoi(input.inputLines[j]);
      for (int k = 0; k < input.lineCount; k++) {
        int c = atoi(input.inputLines[k]);
        if (a + b + c == 2020) {
          outTwo = a * b * c;
        }
      }
    }
  }

  printf("%d", outTwo);

  freeInput(&input);

  return 0;
}

/**
 * Handle freeing all memory being used by input struct.
 */
void freeInput(Input* input) {
  if (input->errCode != 0) {
    free(input->errMessage);
  }

  for (int i = 0; i < input->lineCount; i++) {
    free(input->inputLines[i]);
  }

  free(input->inputLines);
  free(input->inputFilePath);
}

/**
 * Handle loading, parsing, and storing a file and its values into a struct.
 */
void loadInput(char* inputFilePath, Input* input) {
  input->inputFilePath = malloc(strlen(inputFilePath) + 1);
  strcpy(input->inputFilePath, inputFilePath);

  FILE* fHandle = fopen(inputFilePath, "r");

  if (fHandle == NULL) {
    // printf("Error opening file");
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

  rewind(fHandle);

  input->lineLength = maxLineLength;
  input->lineCount = lineCount;

  input->inputLines = malloc(sizeof(char*) * input->lineCount);

  char line[maxLineLength + 2];

  int i = 0;
  while (fgets(line, maxLineLength + 2, fHandle) != NULL) {
    // printf("Line: %s\n", line);
    // printf("strlen: %I64d\n\n", strlen(line));
    input->inputLines[i] = malloc(maxLineLength + 2);
    strcpy(input->inputLines[i], line);
    i++;
  }

  input->errCode = 0;

  fclose(fHandle);
}
