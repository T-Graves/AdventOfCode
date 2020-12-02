#include "main.h"

void day01(Input input) {

  // @ToDo: Need to do error checking to make sure we loaded the file
  //        and stored its values before we attempt to loop over them.
  // @ToDo: Figure out what the more standard / portable version of atoi is.
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
}
