#include "main.h"

/**
 * --- Day 4: Passport Processing ---
 * You arrive at the airport only to realize that you grabbed your North Pole
 * Credentials instead of your passport. While these documents are extremely
 * similar, North Pole Credentials aren't issued by a country and therefore
 * aren't actually valid documentation for travel in most of the world.
 *
 * It seems like you're not the only one having problems, though; a very long
 * line has formed for the automatic passport scanners, and the delay could
 * upset your travel itinerary.
 *
 * Due to some questionable network security, you realize you might be able to
 * solve both of these problems at the same time.
 *
 * The automatic passport scanners are slow because they're having trouble
 * detecting which passports have all required fields. The expected fields are
 * as follows:
 * byr (Birth Year)
 * iyr (Issue Year)
 * eyr (Expiration Year)
 * hgt (Height)
 * hcl (Hair Color)
 * ecl (Eye Color)
 * pid (Passport ID)
 * cid (Country ID)
 *
 * Passport data is validated in batch files (your puzzle input). Each passport
 * is represented as a sequence of key:value pairs separated by spaces or
 * newlines. Passports are separated by blank lines.
 *
 * Here is an example batch file containing four passports:
 * ecl:gry pid:860033327 eyr:2020 hcl:#fffffd
 * byr:1937 iyr:2017 cid:147 hgt:183cm
 *
 * iyr:2013 ecl:amb cid:350 eyr:2023 pid:028048884
 * hcl:#cfa07d byr:1929
 *
 * hcl:#ae17e1 iyr:2013
 * eyr:2024
 * ecl:brn pid:760753108 byr:1931
 * hgt:179cm
 *
 * hcl:#cfa07d eyr:2025 pid:166559648
 * iyr:2011 ecl:brn hgt:59in
 *
 * The first passport is valid - all eight fields are present. The second
 * passport is invalid - it is missing hgt (the Height field).
 *
 * The third passport is interesting; the only missing field is cid, so it looks
 * like data from North Pole Credentials, not a passport at all! Surely, nobody
 * would mind if you made the system temporarily ignore missing cid fields.
 * Treat this "passport" as valid.
 *
 * The fourth passport is missing two fields, cid and byr. Missing cid is fine,
 * but missing any other field is not, so this passport is invalid.
 *
 * According to the above rules, your improved system would report 2 valid
 * passports.
 *
 * Count the number of valid passports - those that have all required fields.
 * Treat cid as optional. In your batch file, how many passports are valid?
 *
 * --- Part Two ---
 * The line is moving more quickly now, but you overhear airport security
 * talking about how passports with invalid data are getting through. Better
 * add some data validation, quick!
 *
 * You can continue to ignore the cid field, but each other field has strict
 * rules about what values are valid for automatic validation:
 * byr (Birth Year) - four digits; at least 1920 and at most 2002.
 * iyr (Issue Year) - four digits; at least 2010 and at most 2020.
 * eyr (Expiration Year) - four digits; at least 2020 and at most 2030.
 * hgt (Height) - a number followed by either cm or in:
 * If cm, the number must be at least 150 and at most 193.
 * If in, the number must be at least 59 and at most 76.
 * hcl (Hair Color) - a # followed by exactly six characters 0-9 or a-f.
 * ecl (Eye Color) - exactly one of: amb blu brn gry grn hzl oth.
 * pid (Passport ID) - a nine-digit number, including leading zeroes.
 * cid (Country ID) - ignored, missing or not.
 *
 * Your job is to count the passports where all required fields are both present
 * and valid according to the above rules. Here are some example values:
 * byr valid:   2002
 * byr invalid: 2003
 *
 * hgt valid:   60in
 * hgt valid:   190cm
 * hgt invalid: 190in
 * hgt invalid: 190
 *
 * hcl valid:   #123abc
 * hcl invalid: #123abz
 * hcl invalid: 123abc
 *
 * ecl valid:   brn
 * ecl invalid: wat
 *
 * pid valid:   000000001
 * pid invalid: 0123456789
 *
 * Here are some invalid passports:
 * eyr:1972 cid:100
 * hcl:#18171d ecl:amb hgt:170 pid:186cm iyr:2018 byr:1926
 *
 * iyr:2019
 * hcl:#602927 eyr:1967 hgt:170cm
 * ecl:grn pid:012533040 byr:1946
 *
 * hcl:dab227 iyr:2012
 * ecl:brn hgt:182cm pid:021572410 eyr:2020 byr:1992 cid:277
 *
 * hgt:59cm ecl:zzz
 * eyr:2038 hcl:74454a iyr:2023
 * pid:3556412378 byr:2007
 * Here are some valid passports:
 *
 * pid:087499704 hgt:74in ecl:grn iyr:2012 eyr:2030 byr:1980
 * hcl:#623a2f
 *
 * eyr:2029 ecl:blu cid:129 byr:1989
 * iyr:2014 pid:896056539 hcl:#a97842 hgt:165cm
 *
 * hcl:#888785
 * hgt:164cm byr:2001 iyr:2015 cid:88
 * pid:545766238 ecl:hzl
 * eyr:2022
 *
 * iyr:2010 hgt:158cm hcl:#b6652a ecl:blu byr:1944 eyr:2021 pid:093154719
 *
 * Count the number of valid passports - those that have all required fields and
 * valid values. Continue to treat cid as optional. In your batch file, how many
 * passports are valid?
 */

typedef struct Passport_t {
  // Expiration Year.
  char* eyr;
  // Issue Year.
  char* iyr;
  // Birth Year.
  char* byr;
  // Eye Color.
  char* ecl;
  // Passport ID.
  char* pid;
  // Hair Color.
  char* hcl;
  // Height.
  char* hgt;
  // Country ID.
  char* cid;
} Passport;

void day04(Input input) {
  int passportCount = 0;

  for (int i = 0; i < input.lineCount; i++) {
    if (input.inputLines[i] == NULL) {
      continue;
    }
    if (input.inputLines[i][0] == ' ' || input.inputLines[i][0] == '\n' || input.inputLines[i][0] == '\r') {
      passportCount++;
      continue;
    }
  }

  Passport** passports = calloc(passportCount, sizeof(Passport*));

  for (int i = 0; i <= passportCount; i++) {
    passports[i] = calloc(1, sizeof(Passport));
    passports[i]->byr = calloc(1, sizeof(char));
    passports[i]->cid = calloc(1, sizeof(char));
    passports[i]->ecl = calloc(1, sizeof(char));
    passports[i]->eyr = calloc(1, sizeof(char));
    passports[i]->hcl = calloc(1, sizeof(char));
    passports[i]->hgt = calloc(1, sizeof(char));
    passports[i]->iyr = calloc(1, sizeof(char));
    passports[i]->pid = calloc(1, sizeof(char));
  }

  int currPassport = 0;
  for (int i = 0; i < input.lineCount; i++) {
    if (input.inputLines[i] == NULL) {
      continue;
    }
    if (input.inputLines[i][0] == ' ' || input.inputLines[i][0] == '\n' || input.inputLines[i][0] == '\r') {
      currPassport++;
      continue;
    }

    char* fieldSavePtr;
    char* valueSavePtr;
    char* token = strtok_r(input.inputLines[i], " ", &fieldSavePtr);

    while (token != NULL) {
      char* field = token;

      char* key = strtok_r(field, ":", &valueSavePtr);
      char* value = strtok_r(NULL, ":", &valueSavePtr);

      if (strcmp(key, "eyr") == 0) {
        passports[currPassport]->eyr = realloc(passports[currPassport]->eyr, strcspn(value, "\r\n") + 1);
        strncpy(passports[currPassport]->eyr, value, strcspn(value, "\r\n"));
        passports[currPassport]->eyr[strcspn(value, "\r\n")] = '\0';
      }
      else if (strcmp(key, "iyr") == 0) {
        passports[currPassport]->iyr = realloc(passports[currPassport]->iyr, strcspn(value, "\r\n") + 1);
        strncpy(passports[currPassport]->iyr, value, strcspn(value, "\r\n"));
        passports[currPassport]->iyr[strcspn(value, "\r\n")] = '\0';
      }
      else if (strcmp(key, "byr") == 0) {
        passports[currPassport]->byr = realloc(passports[currPassport]->byr, strcspn(value, "\r\n") + 1);
        strncpy(passports[currPassport]->byr, value, strcspn(value, "\r\n"));
        passports[currPassport]->byr[strcspn(value, "\r\n")] = '\0';
      }
      else if (strcmp(key, "ecl") == 0) {
        passports[currPassport]->ecl = realloc(passports[currPassport]->ecl, strcspn(value, "\r\n") + 1);
        strncpy(passports[currPassport]->ecl, value, strcspn(value, "\r\n"));
        passports[currPassport]->ecl[strcspn(value, "\r\n")] = '\0';
      }
      else if (strcmp(key, "pid") == 0) {
        passports[currPassport]->pid = realloc(passports[currPassport]->pid, strcspn(value, "\r\n") + 1);
        strncpy(passports[currPassport]->pid, value, strcspn(value, "\r\n"));
        passports[currPassport]->pid[strcspn(value, "\r\n")] = '\0';
      }
      else if (strcmp(key, "hcl") == 0) {
        passports[currPassport]->hcl = realloc(passports[currPassport]->hcl, strcspn(value, "\r\n") + 1);
        strncpy(passports[currPassport]->hcl, value, strcspn(value, "\r\n"));
        passports[currPassport]->hcl[strcspn(value, "\r\n")] = '\0';
      }
      else if (strcmp(key, "hgt") == 0) {
        passports[currPassport]->hgt = realloc(passports[currPassport]->hgt, strcspn(value, "\r\n") + 1);
        strncpy(passports[currPassport]->hgt, value, strcspn(value, "\r\n"));
        passports[currPassport]->hgt[strcspn(value, "\r\n")] = '\0';
      }
      else if (strcmp(key, "cid") == 0) {
        passports[currPassport]->cid = realloc(passports[currPassport]->cid, strcspn(value, "\r\n") + 1);
        strncpy(passports[currPassport]->cid, value, strcspn(value, "\r\n"));
        passports[currPassport]->cid[strcspn(value, "\r\n")] = '\0';
      }

      token = strtok_r(NULL, " ", &fieldSavePtr);
    }
  }

  int validPassportsPtOne = 0;
  for (int i = 0; i <= passportCount; i++) {
    // Eyr validation.
    if (strcmp(passports[i]->eyr, "") == 0) {
      continue;
    }
    // Iyr validation.
    if (strcmp(passports[i]->iyr, "") == 0) {
      continue;
    }
    // Byr validation.
    if (strcmp(passports[i]->byr, "") == 0) {
      continue;
    }
    // Ecl validation.
    if (strcmp(passports[i]->ecl, "") == 0) {
      continue;
    }
    // Pid validation.
    if (strcmp(passports[i]->pid, "") == 0) {
      continue;
    }
    // Hcl validation.
    if (strcmp(passports[i]->hcl, "") == 0) {
      continue;
    }
    // Hgt validaiton.
    if (strcmp(passports[i]->hgt, "") == 0) {
      continue;
    }
    // Cid validaiton.
    if (strcmp(passports[i]->cid, "") == 0) {
      // cid optional.
    }

    validPassportsPtOne++;
  }

  printf("Part 1: %d\n", validPassportsPtOne);

  int validPassportsPtTwo = 0;
  for (int i = 0; i <= passportCount; i++) {

    // Eyr validation.
    // eyr (Expiration Year) - four digits; at least 2020 and at most 2030.
    if (strcmp(passports[i]->eyr, "") == 0) {
      continue;
    }
    else if (strlen(passports[i]->eyr) != 4) {
      continue;
    }
    else if (strtol(passports[i]->eyr, NULL, 0) < 2020 || strtol(passports[i]->eyr, NULL, 0) > 2030) {
      continue;
    }

    // Iyr validation.
    // iyr (Issue Year) - four digits; at least 2010 and at most 2020.
    if (strcmp(passports[i]->iyr, "") == 0) {
      continue;
    }
    else if (strlen(passports[i]->iyr) != 4) {
      continue;
    }
    else if (strtol(passports[i]->iyr, NULL, 0) < 2010 || strtol(passports[i]->iyr, NULL, 0) > 2020) {
      continue;
    }

    // Byr validation.
    // byr (Birth Year) - four digits; at least 1920 and at most 2002.
    if (strcmp(passports[i]->byr, "") == 0) {
      continue;
    }
    else if (strlen(passports[i]->byr) != 4) {
      continue;
    }
    else if (strtol(passports[i]->byr, NULL, 0) < 1920 || strtol(passports[i]->byr, NULL, 0) > 2002) {
      continue;
    }

    // Ecl validation.
    // ecl (Eye Color) - exactly one of: amb blu brn gry grn hzl oth.
    if (strcmp(passports[i]->ecl, "") == 0) {
      continue;
    }
    else {
      char col[][4] = {
        "amb",
        "blu",
        "brn",
        "gry",
        "grn",
        "hzl",
        "oth",
        "\0"
      };

      bool validColor = false;
      int colIndex = 0;
      while (strcmp(col[colIndex], "\0") != 0) {
        if (strcmp(col[colIndex], passports[i]->ecl) == 0) {
          validColor = true;
          break;
        }
        colIndex++;
      }

      if (!validColor) {
        continue;
      }
    }

    // Pid validation.
    // pid (Passport ID) - a nine-digit number, including leading zeroes.
    if (strcmp(passports[i]->pid, "") == 0) {
      continue;
    }
    else if (strlen(passports[i]->pid) != 9) {
      continue;
    }
    else if (strtol(passports[i]->pid, NULL, 10) == 0L) {
      continue;
    }

    // Hcl validation.
    // hcl (Hair Color) - a # followed by exactly six characters 0-9 or a-f.
    if (strcmp(passports[i]->hcl, "") == 0) {
      continue;
    }
    else if (strlen(passports[i]->hcl) != 7) {
      continue;
    }
    else {
      if (passports[i]->hcl[0] != '#') {
        continue;
      }

      bool validHairColor = true;
      for (int j = 1; j < 7; j++) {
        if (!(passports[i]->hcl[j] >= '0' && passports[i]->hcl[j] <= '9')) {
          if (!(passports[i]->hcl[j] >= 'a' && passports[i]->hcl[j] <= 'f')) {
            validHairColor = false;
          }
        }

        if (!validHairColor) {
          continue;
        }
      }
    }

    // Hgt validaiton.
    // hgt (Height) - a number followed by either cm or in:
    // If cm, the number must be at least 150 and at most 193.
    // If in, the number must be at least 59 and at most 76.
    if (strcmp(passports[i]->hgt, "") == 0) {
      continue;
    }
    else {
      int height = strtol(passports[i]->hgt, NULL, 0);

      if (strstr(passports[i]->hgt, "cm") != NULL) {
        if (height < 150 || height > 193) {
          continue;
        }
      }
      else if (strstr(passports[i]->hgt, "in") != NULL) {
        if (height < 59 || height > 76) {
          continue;
        }
      }
      else {
        // Invalid format.
        continue;
      }
    }

    // Cid validaiton.
    if (strcmp(passports[i]->cid, "") == 0) {
      // cid optional.
    }

    validPassportsPtTwo++;
  }

  printf("Part 2: %d\n", validPassportsPtTwo);

  for (int i = 0; i < passportCount; i++) {
    free(passports[i]->byr);
    free(passports[i]->cid);
    free(passports[i]->ecl);
    free(passports[i]->eyr);
    free(passports[i]->hcl);
    free(passports[i]->hgt);
    free(passports[i]->iyr);
    free(passports[i]->pid);
  }
  free(passports);
}
