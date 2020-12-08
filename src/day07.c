#include "main.h"

/**
 * --- Day 7: Handy Haversacks ---
 * You land at the regional airport in time for your next flight. In fact, it
 * looks like you'll even have time to grab some food: all flights are currently
 * delayed due to issues in luggage processing.
 *
 * Due to recent aviation regulations, many rules (your puzzle input) are being
 * enforced about bags and their contents; bags must be color-coded and must
 * contain specific quantities of other color-coded bags. Apparently, nobody
 * responsible for these regulations considered how long they would take to
 * enforce!
 *
 * For example, consider the following rules:
 * light red bags contain 1 bright white bag, 2 muted yellow bags.
 * dark orange bags contain 3 bright white bags, 4 muted yellow bags.
 * bright white bags contain 1 shiny gold bag.
 * muted yellow bags contain 2 shiny gold bags, 9 faded blue bags.
 * shiny gold bags contain 1 dark olive bag, 2 vibrant plum bags.
 * dark olive bags contain 3 faded blue bags, 4 dotted black bags.
 * vibrant plum bags contain 5 faded blue bags, 6 dotted black bags.
 * faded blue bags contain no other bags.
 * dotted black bags contain no other bags.
 *
 * These rules specify the required contents for 9 bag types. In this example,
 * every faded blue bag is empty, every vibrant plum bag contains 11 bags
 * (5 faded blue and 6 dotted black), and so on.
 *
 * You have a shiny gold bag. If you wanted to carry it in at least one other
 * bag, how many different bag colors would be valid for the outermost bag?
 * (In other words: how many colors can, eventually, contain at least one
 * shiny gold bag?)
 *
 * In the above rules, the following options would be available to you:
 * A bright white bag, which can hold your shiny gold bag directly.
 * A muted yellow bag, which can hold your shiny gold bag directly,
 * plus some other bags.
 * A dark orange bag, which can hold bright white and muted yellow bags,
 * either of which could then hold your shiny gold bag.
 * A light red bag, which can hold bright white and muted yellow bags,
 * either of which could then hold your shiny gold bag.
 *
 * So, in this example, the number of bag colors that can eventually contain at
 * least one shiny gold bag is 4.
 *
 * How many bag colors can eventually contain at least one shiny gold bag? (The
 * list of rules is quite long; make sure you get all of it.)
 *
 * --- Part Two ---
 * It's getting pretty expensive to fly these days - not because of ticket
 * prices, but because of the ridiculous number of bags you need to buy!
 *
 * Consider again your shiny gold bag and the rules from the above example:
 * faded blue bags contain 0 other bags.
 * dotted black bags contain 0 other bags.
 * vibrant plum bags contain 11 other bags: 5 faded blue bags and 6 dotted
 * black bags.
 * dark olive bags contain 7 other bags: 3 faded blue bags and 4 dotted
 * black bags.
 *
 * So, a single shiny gold bag must contain 1 dark olive bag (and the 7 bags
 * within it) plus 2 vibrant plum bags (and the 11 bags within each of those):
 * 1 + 1*7 + 2 + 2*11 = 32 bags!
 *
 * Of course, the actual rules have a small chance of going several levels
 * deeper than this example; be sure to count all of the bags, even if the
 * nesting becomes topologically impractical!
 *
 * Here's another example:
 * shiny gold bags contain 2 dark red bags.
 * dark red bags contain 2 dark orange bags.
 * dark orange bags contain 2 dark yellow bags.
 * dark yellow bags contain 2 dark green bags.
 * dark green bags contain 2 dark blue bags.
 * dark blue bags contain 2 dark violet bags.
 * dark violet bags contain no other bags.
 *
 * In this example, a single shiny gold bag must contain 126 other bags.
 *
 * How many individual bags are required inside your single shiny gold bag?
 */

typedef struct Bag_st Bag;
typedef struct Bag_restriction_st Bag_restriction;
typedef struct Bag_list_st Bag_list;

struct Bag_st {
  char *descriptor;
  char *color;
  Bag_restriction *restrictions;
  int restrict_list_size;
  int restrict_count;
  bool is_new;
};

struct Bag_restriction_st {
  Bag *bag;
  int limit;
};

struct Bag_list_st {
  Bag **bags;
  int bag_count;
};

void init_bag_list(Bag_list *, int);
void bag_list_add(Bag_list *, Bag *);
Bag *bag_list_search(Bag_list *, char *, char *);
void free_bag_list(Bag_list *);

Bag *create_bag(char *, char *);
void bag_add_restriction(Bag *, Bag_restriction);
void free_bag(Bag *);

Bag_restriction create_bag_restriction(Bag *, int);
void free_bag_restriction(Bag_restriction);

bool contains_shiny_gold_bag(Bag *);
int count_sub_bags(Bag *);

void day07(Input input) {
  Bag_list bags;
  init_bag_list(&bags, input.lineCount);

  for (int i = 0; i < input.lineCount; i++) {
    if (input.inputLines[i] == NULL
      || input.inputLines[i][0] == ' '
      || input.inputLines[i][0] == '\n'
      || input.inputLines[i][0] == '\r') {
      continue;
    }

    char *containPivot = strstr(input.inputLines[i], " contain ");

    char *bagDef = calloc((containPivot - input.inputLines[i]) + 1, sizeof(char));
    if (bagDef == NULL) {
      printf("calloc error");
      break;
    }
    else {
      strncpy(bagDef, input.inputLines[i], containPivot - input.inputLines[i]);
      bagDef[(containPivot - input.inputLines[i])] = '\0';

      char *containedBags = containPivot + (strlen(" contain "));

      char *bagDesc = strtok(bagDef, " ");
      char *bagColor = strtok(NULL, " ");
      Bag *outerBag = bag_list_search(&bags, bagDesc, bagColor);
      if (outerBag->is_new) {
        bag_list_add(&bags, outerBag);
      }

      char *containedBagsPtr;
      char *containedBag = strtok_r(containedBags, ",", &containedBagsPtr);
      while (containedBag != NULL) {
        char *containedBagPtr;
        char *containedBagLimit = strtok_r(containedBag, " ", &containedBagPtr);
        char *containedBagDesc = strtok_r(NULL, " ", &containedBagPtr);
        char *containedBagColor = strtok_r(NULL, " ", &containedBagPtr);

        // @ToDo: Handle "no other bags."
        if (strcmp(containedBagLimit, "no") == 0) {
          break;
        }

        Bag *innerBag = bag_list_search(&bags, containedBagDesc, containedBagColor);
        if (innerBag->is_new) {
          bag_list_add(&bags, innerBag);
        }

        Bag_restriction restriction = create_bag_restriction(innerBag, (int)strtol(containedBagLimit, NULL, 0));
        bag_add_restriction(outerBag, restriction);

        containedBag = strtok_r(NULL, ",", &containedBagsPtr);
      }

      free(bagDef);
    }
  }

  int contains_gold_bag = 0;
  int shiny_gold_bag_index = 0;
  for (int i = 0; i < bags.bag_count; i++) {
    if (strcmp(bags.bags[i]->descriptor, "shiny") == 0
      && strcmp(bags.bags[i]->color, "gold") == 0) {
      shiny_gold_bag_index = i;
    }
    if (contains_shiny_gold_bag(bags.bags[i])) {
      contains_gold_bag++;
    }
  }
  printf("Part 1: %d\n", contains_gold_bag);

  int sub_bag_count = count_sub_bags(bags.bags[shiny_gold_bag_index]) - 1;
  printf("Part 2: %d\n", sub_bag_count);

  free_bag_list(&bags);
}

void init_bag_list(Bag_list *bag_list, int size) {
  bag_list->bags = calloc(size, sizeof(Bag *));
  bag_list->bag_count = 0;
}
void bag_list_add(Bag_list *bag_list, Bag *bag) {
  bag_list->bags[bag_list->bag_count] = bag;
  bag_list->bag_count++;
}
Bag *bag_list_search(Bag_list *bag_list, char *descriptor, char *color) {
  for (int i = 0; i < bag_list->bag_count; i++) {
    if (strcmp(bag_list->bags[i]->descriptor, descriptor) == 0
      && strcmp(bag_list->bags[i]->color, color) == 0) {
      bag_list->bags[i]->is_new = false;
      return bag_list->bags[i];
    }
  }

  Bag *new_bag = create_bag(descriptor, color);
  return new_bag;
}
void free_bag_list(Bag_list *bag_list) {
  for (int i = 0; i < bag_list->bag_count; i++) {
    free_bag(bag_list->bags[i]);
  }
  free(bag_list->bags);
}

Bag *create_bag(char *descriptor, char *color) {
  Bag *bag = malloc(sizeof(Bag));

  bag->descriptor = calloc(strlen(descriptor) + 1, sizeof(char));
  strcpy(bag->descriptor, descriptor);

  bag->color = calloc(strlen(color) + 1, sizeof(char));
  strcpy(bag->color, color);

  bag->restrict_list_size = 10;
  bag->restrictions = calloc(bag->restrict_list_size, sizeof(Bag_restriction));
  bag->restrict_count = 0;

  bag->is_new = true;

  return bag;
}
void bag_add_restriction(Bag *bag, Bag_restriction restriction) {
  bag->restrictions[bag->restrict_count] = restriction;
  bag->restrict_count++;
}
void free_bag(Bag *bag) {
  free(bag->restrictions);
  free(bag->color);
  free(bag->descriptor);
  free(bag);
}

Bag_restriction create_bag_restriction(Bag *bag, int limit) {
  Bag_restriction restriction;
  restriction.bag = bag;
  restriction.limit = limit;
  return restriction;
}

bool contains_shiny_gold_bag(Bag *bag) {
  bool contains_bag = false;
  if (bag->restrict_count > 0) {
    for (int i = 0; i < bag->restrict_count; i++) {
      if (strcmp(bag->restrictions[i].bag->descriptor, "shiny") == 0 && strcmp(bag->restrictions[i].bag->color, "gold") == 0) {
        contains_bag = true;
        break;
      }
      else {
        contains_bag = contains_shiny_gold_bag(bag->restrictions[i].bag);
        if (contains_bag) {
          break;
        }
      }
    }
  }

  return contains_bag;
}

int count_sub_bags(Bag *bag) {
  int sub_bags = 1;
  if (bag->restrict_count > 0) {
    for (int i = 0; i < bag->restrict_count; i++) {
      sub_bags += (bag->restrictions[i].limit * count_sub_bags(bag->restrictions[i].bag));
    }
  }

  return sub_bags;
}
