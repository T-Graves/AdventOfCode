#include <stdint.h>
#include "main.h"

/**
 * --- Day 2: Password Philosophy ---
 * Your flight departs in a few days from the coastal airport; the easiest way
 * down to the coast from here is via toboggan.
 *
 * The shopkeeper at the North Pole Toboggan Rental Shop is having a bad day.
 * "Something's wrong with our computers; we can't log in!"
 * You ask if you can take a look.
 *
 * Their password database seems to be a little corrupted: some of the
 * passwords wouldn't have been allowed by the Official Toboggan Corporate
 * Policy that was in effect when they were chosen.
 *
 * To try to debug the problem, they have created a list (your puzzle input)
 * of passwords (according to the corrupted database) and the corporate policy
 * when that password was set.
 *
 * For example, suppose you have the following list:
 * 1-3 a: abcde
 * 1-3 b: cdefg
 * 2-9 c: ccccccccc
 *
 * Each line gives the password policy and then the password. The password
 * policy indicates the lowest and highest number of times a given letter
 * must appear for the password to be valid. For example, 1-3 a means that
 * the password must contain a at least 1 time and at most 3 times.
 *
 * In the above example, 2 passwords are valid.
 * The middle password, cdefg, isnot; it contains no instances of b, but
 * needs at least 1. The first and third passwords are valid: they contain
 * one a or nine c, both within the limits of their respective policies.
 *
 * How many passwords are valid according to their policies?
 *
 * --- Part Two ---
 * While it appears you validated the passwords correctly, they don't seem to be
 * what the Official Toboggan Corporate Authentication System is expecting.
 *
 * The shopkeeper suddenly realizes that he just accidentally explained the
 * password policy rules from his old job at the sled rental place down the
 * street! The Official Toboggan Corporate Policy actually works a little
 * differently.
 *
 * Each policy actually describes two positions in the password, where 1 means
 * the first character, 2 means the second character, and so on. (Be careful;
 * Toboggan Corporate Policies have no concept of "index zero"!) Exactly one of
 * these positions must contain the given letter. Other occurrences of the
 * letter are irrelevant for the purposes of policy enforcement.
 *
 * Given the same example list from above:
 * 1-3 a: abcde is valid: position 1 contains a and position 3 does not.
 * 1-3 b: cdefg is invalid: neither position 1 nor position 3 contains b.
 * 2-9 c: ccccccccc is invalid: both position 2 and position 9 contain c.
 *
 * How many passwords are valid according to the new interpretation of the policies?
 */

void day02(Input input) {
  int validPasswordsPartOne = 0;
  int validPasswordsPartTwo = 0;
  for (int i = 0; i < input.lineCount; i++) {

    // Separate input string on the ':' char.
    char *policy = strtok(input.inputLines[i], ":");
    char *password = (strtok(NULL, ":") + 1) + '\0';

    // Separate the policy string on the ' ' char.
    char *policyValidators = strtok(policy, " ");
    char *policyLetterString = strtok(NULL, " ");
    // Check if the policy letter string is null. This is probably a blank line.
    if (policyLetterString == NULL) {
      continue;
    }

    // Get the actual policy letter char.
    char policyLetter = policyLetterString[0];

    // Separate the first and second validators.
    int policyValidatorOne = strtol(strtok(policyValidators, "-"), NULL, 0);
    int policyValidatorTwo = strtol(strtok(NULL, "-"), NULL, 0);

    // Make sure neither of them are zero.
    if (policyValidatorOne == 0 || policyValidatorTwo == 0) {
      continue;
    }

    // Count how many of the policy letters are in the password for answer one.
    int policyLetterCount = 0;
    for (int j = 0; j <= (int)strlen(password); j++) {
      if (password[j] == policyLetter) {
        policyLetterCount++;
      }
    }
    // Check that the policyLetterCount is within the validator boundaries.
    if (policyLetterCount >= policyValidatorOne && policyLetterCount <= policyValidatorTwo) {
      validPasswordsPartOne++;
    }

    // Check that one of the two validator positions in the password contain
    // the policy letter. Only one or the other, but not both.
    if ((password[policyValidatorOne - 1] == policyLetter && password[policyValidatorTwo - 1] != policyLetter)
        || (password[policyValidatorOne - 1] != policyLetter && password[policyValidatorTwo - 1] == policyLetter)) {
      validPasswordsPartTwo++;
    }
  }

  printf("Part 1: %d\n", validPasswordsPartOne);
  printf("Part 2: %d", validPasswordsPartTwo);
}
