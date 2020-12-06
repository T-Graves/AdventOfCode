#include "main.h"

/**
 * --- Day 6: Custom Customs ---
 * As your flight approaches the regional airport where you'll switch to a much
 * larger plane, customs declaration forms are distributed to the passengers.
 *
 * The form asks a series of 26 yes-or-no questions marked a through z. All you
 * need to do is identify the questions for which anyone in your group answers
 * "yes". Since your group is just you, this doesn't take very long.
 *
 * However, the person sitting next to you seems to be experiencing a language
 * barrier and asks if you can help. For each of the people in their group, you
 * write down the questions for which they answer "yes", one per line.
 * For example:
 * abcx
 * abcy
 * abcz
 *
 * In this group, there are 6 questions to which anyone answered "yes":
 * a, b, c, x, y, and z.
 * (Duplicate answers to the same question don't count extra; each question
 * counts at most once.)
 *
 * Another group asks for your help, then another, and eventually you've
 * collected answers from every group on the plane (your puzzle input).
 * Each group's answers are separated by a blank line, and within each group,
 * each person's answers are on a single line.
 * For example:
 * abc
 *
 * a
 * b
 * c
 *
 * ab
 * ac
 *
 * a
 * a
 * a
 * a
 *
 * b
 *
 * This list represents answers from five groups:
 *
 * The first group contains one person who answered "yes" to
 * 3 questions: a, b, and c.
 * The second group contains three people; combined, they answered "yes" to
 * 3 questions: a, b, and c.
 * The third group contains two people; combined, they answered "yes" to
 * 3 questions: a, b, and c.
 * The fourth group contains four people; combined, they answered "yes" to only
 * 1 question, a.
 * The last group contains one person who answered "yes" to only
 * 1 question, b.
 * In this example, the sum of these counts is 3 + 3 + 3 + 1 + 1 = 11.
 *
 * For each group, count the number of questions to which anyone answered "yes".
 * What is the sum of those counts?
 *
 * --- Part Two ---
 * As you finish the last group's customs declaration, you notice that you
 * misread one word in the instructions:
 *
 * You don't need to identify the questions to which anyone answered "yes";
 * you need to identify the questions to which everyone answered "yes"!
 *
 * Using the same example as above:
 * abc
 *
 * a
 * b
 * c
 *
 * ab
 * ac
 *
 * a
 * a
 * a
 * a
 *
 * b
 *
 * This list represents answers from five groups:
 * In the first group, everyone (all 1 person) answered "yes" to
 * 3 questions: a, b, and c.
 * In the second group, there is no question to which everyone answered "yes".
 * In the third group, everyone answered yes to only 1 question, a.
 * Since some people did not answer "yes" to b or c, they don't count.
 * In the fourth group, everyone answered yes to only 1 question, a.
 * In the fifth group, everyone (all 1 person) answered "yes" to 1 question, b.
 * In this example, the sum of these counts is 3 + 0 + 1 + 1 + 1 = 6.
 *
 * For each group, count the number of questions to which everyone answered
 * "yes". What is the sum of those counts?
 */

void day06(Input input) {
  // Figure out how many groups there are.
  int groupCount = 0;
  for (int i = 0; i < input.lineCount; i++) {
    // Check if this is a blank line. Blank lines separate groups.
    if (input.inputLines[i] == NULL
      || input.inputLines[i][0] == ' '
      || input.inputLines[i][0] == '\n'
      || input.inputLines[i][0] == '\r') {

      groupCount++;
      continue;
    }
  }

  // Prepare an array to store the answer characters.
  char groupAnswers[groupCount][input.lineLength * 2];
  // Initialize the arrays to all null terminators.
  for (int i = 0; i < groupCount; i++) {
    for (int j = 0; j < input.lineLength * 2; j++) {
      groupAnswers[i][j] = '\0';
    }
  }

  int yesSum = 0;
  int currGroup = 0;
  int currAnswerCount = 0;
  for (int i = 0; i < input.lineCount; i++) {
    // Check if this is a blank line. Blank lines separate groups.
    // Once we hit a group separator, add the current answer count to the total
    // increment group number, and reset currAnswerCount.
    if (input.inputLines[i] == NULL
      || input.inputLines[i][0] == ' '
      || input.inputLines[i][0] == '\n'
      || input.inputLines[i][0] == '\r') {

      yesSum += currAnswerCount;

      currGroup++;
      currAnswerCount = 0;
      continue;
    }

    // Check each character and add it to the groupAnswers string if it isn't
    // already in it.
    int index = 0;
    while(input.inputLines[i][index] != '\n') {
      if (!strchr(groupAnswers[currGroup], input.inputLines[i][index])) {
        groupAnswers[currGroup][currAnswerCount] = input.inputLines[i][index];
        currAnswerCount++;
      }
      index++;
    }
  }

  printf("Part 1: %d\n", yesSum);

  /**
   * This next part is gross IMO. Pretty sure this is a terrible way to do this,
   * but it worked so whatever.
   */

  // Reset groupAnswers strings.
  for (int i = 0; i < groupCount; i++) {
    for (int j = 0; j < input.lineLength * 2; j++) {
      groupAnswers[i][j] = '\0';
    }
  }

  int fullYesSum = 0;
  currGroup = 0;
  int numInGroup = 0;
  for (int i = 0; i < input.lineCount; i++) {
    // Check if this is a blank line. Blank lines separate groups.
    if (input.inputLines[i] == NULL
      || input.inputLines[i][0] == ' '
      || input.inputLines[i][0] == '\n'
      || input.inputLines[i][0] == '\r') {

      // Loop over groupAnswer string for this group and check the totals
      // for each character. If they match the numInGroup value, we know
      // this answer was answered yes by everyone in the group.
      int numfullYes = 0;
      int charPos = 0;
      while (groupAnswers[currGroup][charPos] != '\0') {
        if ((int)groupAnswers[currGroup][charPos + 1] == numInGroup) {
          numfullYes++;
        }
        charPos += 2;
      }

      fullYesSum += numfullYes;

      currGroup++;
      numInGroup = 0;

      continue;
    }
    // Keep track of the number in this group.
    numInGroup++;

    // If there is nothing in this groupAnswers string yet, add every character
    // of the current input line to it, along with a count of 1. Each character
    // will be stored as pairs like {char}{count}.
    if (strlen(groupAnswers[currGroup]) == 0) {
      int index = 0;
      int charPos = 0;
      while (input.inputLines[i][index] != '\n') {
        groupAnswers[currGroup][charPos] = input.inputLines[i][index];
        groupAnswers[currGroup][charPos + 1] = 1;
        index++;
        charPos += 2;
      }
    }
    else {
      // Loop over input line characters and check if they are in the
      // groupAnswers string. If the character is found, increment its count.
      for (int j = 0; j < strlen(input.inputLines[i]); j++){
        char* charPos = strchr(groupAnswers[currGroup], input.inputLines[i][j]);
        if (charPos) {
          charPos[1]++;
        }
      }
    }
  }

  printf("Part 2: %d\n", fullYesSum);
}
