#include "main.h"

#define BC_INIT_BASE_SIZE 50

/**
 * --- Day 8: Handheld Halting ---
 * Your flight to the major airline hub reaches cruising altitude without
 * incident. While you consider checking the in-flight menu for one of those
 * drinks that come with a little umbrella, you are interrupted by the kid
 * sitting next to you.
 *
 * Their handheld game console won't turn on! They ask if you can take a look.
 *
 * You narrow the problem down to a strange infinite loop in the boot code
 * (your puzzle input) of the device. You should be able to fix it, but first
 * you need to be able to run the code in isolation.
 *
 * The boot code is represented as a text file with one instruction per line of
 * text. Each instruction consists of an operation (acc, jmp, or nop) and an
 * argument (a signed number like +4 or -20).
 *
 * acc increases or decreases a single global value called the accumulator by
 * the value given in the argument. For example, acc +7 would increase the
 * accumulator by 7. The accumulator starts at 0. After an acc instruction,
 * the instruction immediately below it is executed next.
 *
 * jmp jumps to a new instruction relative to itself. The next instruction to
 * execute is found using the argument as an offset from the jmp instruction;
 * for example, jmp +2 would skip the next instruction, jmp +1 would continue
 * to the instruction immediately below it, and jmp -20 would cause the
 * instruction 20 lines above to be executed next.
 *
 * nop stands for No OPeration - it does nothing. The instruction immediately
 * below it is executed next.
 *
 * For example, consider the following program:
 * nop +0
 * acc +1
 * jmp +4
 * acc +3
 * jmp -3
 * acc -99
 * acc +1
 * jmp -4
 * acc +6
 *
 * These instructions are visited in this order:
 * nop +0  | 1
 * acc +1  | 2, 8(!)
 * jmp +4  | 3
 * acc +3  | 6
 * jmp -3  | 7
 * acc -99 |
 * acc +1  | 4
 * jmp -4  | 5
 * acc +6  |
 *
 * First, the nop +0 does nothing. Then, the accumulator is increased from 0
 * to 1 (acc +1) and jmp +4 sets the next instruction to the other acc +1 near
 * the bottom. After it increases the accumulator from 1 to 2, jmp -4 executes,
 * setting the next instruction to the only acc +3. It sets the accumulator to
 * 5, and jmp -3 causes the program to continue back at the first acc +1.
 *
 * This is an infinite loop: with this sequence of jumps, the program will run
 * forever. The moment the program tries to run any instruction a second time,
 * you know it will never terminate.
 *
 * Immediately before the program would run an instruction a second time, the
 * value in the accumulator is 5.
 *
 * Run your copy of the boot code. Immediately before any instruction is
 * executed a second time, what value is in the accumulator?
 *
 * --- Part Two ---
 * After some careful analysis, you believe that exactly one instruction is
 * corrupted.
 *
 * Somewhere in the program, either a jmp is supposed to be a nop, or a nop is
 * supposed to be a jmp. (No acc instructions were harmed in the corruption of
 * this boot code.)
 *
 * The program is supposed to terminate by attempting to execute an instruction
 * immediately after the last instruction in the file. By changing exactly one
 * jmp or nop, you can repair the boot code and make it terminate correctly.
 *
 * For example, consider the same program from above:
 * nop +0
 * acc +1
 * jmp +4
 * acc +3
 * jmp -3
 * acc -99
 * acc +1
 * jmp -4
 * acc +6
 *
 * If you change the first instruction from nop +0 to jmp +0, it would create a
 * single-instruction infinite loop, never leaving that instruction. If you
 * change almost any of the jmp instructions, the program will still eventually
 * find another jmp instruction and loop forever.
 *
 * However, if you change the second-to-last instruction
 * (from jmp -4 to nop -4), the program terminates! The instructions are visited
 * in this order:
 * nop +0  | 1
 * acc +1  | 2
 * jmp +4  | 3
 * acc +3  |
 * jmp -3  |
 * acc -99 |
 * acc +1  | 4
 * nop -4  | 5
 * acc +6  | 6
 *
 * After the last instruction (acc +6), the program terminates by attempting to
 * run the instruction below the last instruction in the file. With this change,
 * after the program terminates, the accumulator contains the value 8 (acc +1,
 * acc +1, acc +6).
 *
 * Fix the program so that it terminates normally by changing exactly one jmp
 * (to nop) or nop (to jmp). What is the value of the accumulator after the
 * program terminates?
 */

typedef struct OpCode_st Opc;
typedef struct BootCode_st BootCode;

void init_boot_code(BootCode *bootCode, const int baseSize);
void free_boot_code(BootCode *bootCode);
void increase_boot_code_array(BootCode *bootCode);
void add_op(BootCode *bootCode, char *op, long arg);

bool runCode(BootCode *bootCode, long *accumulator);
void resetRunCounts(BootCode *bootCode);

struct OpCode_st {
  char *op;
  long arg;
  int runCount;
};

struct BootCode_st {
  Opc **ops;
  int size;
  int count;
};

void day08(Input input) {
  BootCode code;
  // Init code struct to an initial base size of 50 opcodes.
  init_boot_code(&code, BC_INIT_BASE_SIZE);

  for (int i = 0; i < input.lineCount; i++) {
    if (input.inputLines[i] == NULL
      || input.inputLines[i][0] == ' '
      || input.inputLines[i][0] == '\n'
      || input.inputLines[i][0] == '\r') {
      continue;
    }

    // Grab the op code from the current line.
    char *opString = strtok(input.inputLines[i], " ");
    // Grab the op arg from the current line as a string.
    char *argString = strtok(NULL, " ");
    // Convert the arg string into a long.
    long arg;
    stol(argString, &arg);

    // Add the opcode to the op code list.
    add_op(&code, opString, arg);
  }

  // Initialize an accumulator.
  long accumulator = 0;
  // Run the code as it currently is. runCode will return true if an infinite
  // loop occures (i.e. an opcode was hit more than once).
  bool isInfLoop = runCode(&code, &accumulator);
  // Output value of accumulator at the point we hit an already run opcode.
  printf("Part 1: %ld\n", accumulator);
  printf("Part 1 Inf Loop Found? %s\n", isInfLoop ? "Yes" : "No");

  // Loop over opcode list.
  for (int i = 0; i < code.count; i++) {
    // Skip "acc". They are fine.
    if (strcmp(code.ops[i]->op, "acc") == 0) {
      continue;
    }

    // Check if this is a "nop" code.
    if (strcmp(code.ops[i]->op, "nop") == 0) {
      // If this "nop" codes arg value is 0, skip. Turning it into a "jmp" will
      // get us stuck in a loop.
      if (code.ops[i]->arg == 0) {
        continue;
      }

      // Clear out the "nop" code and replace it with "jmp".
      free(code.ops[i]->op);
      code.ops[i]->op = strdup("jmp");

      // Reset the accumulator.
      accumulator = 0;
      // Attempt to run the code.
      isInfLoop = runCode(&code, &accumulator);
      // Check if we got into a loop.
      if (isInfLoop) {
        // Clear out the "jmp" and readd the "nop".
        free(code.ops[i]->op);
        code.ops[i]->op = strdup("nop");

        // Continue on.
        continue;
      }
      else {
        // We found the corrupted opcode.
        break;
      }
    }

    // Check if this is a "jmp" code.
    if (strcmp(code.ops[i]->op, "jmp") == 0) {
      // Clear out the "jmp" and replace it with "nop".
      free(code.ops[i]->op);
      code.ops[i]->op = strdup("nop");

      // Reset accumulator.
      accumulator = 0;
      // Attempt to run the code.
      isInfLoop = runCode(&code, &accumulator);
      // Check if we got into a loop.
      if (isInfLoop) {
        // Clear out the "nop" and readd the "jmp".
        free(code.ops[i]->op);
        code.ops[i]->op = strdup("jmp");

        // Continue on.
        continue;
      }
      else {
        // We found the corrupted opcode.
        break;
      }
    }
  }

  // Output accumulator value at the end of the "program".
  printf("Part 2: %ld\n", accumulator);
  printf("Part 2 Inf Loop Found? %s\n", isInfLoop ? "Yes" : "No");

  // Free bootcode struct.
  free_boot_code(&code);
}

/**
 * Run code.
 *
 * @param BootCode *bootcode
 *   The opcode list to run through.
 * @param long *accumulator
 *   The accumulator to use.
 */
bool runCode(BootCode *bootCode, long *accumulator) {
  // Set up program counter.
  long programCounter = 0;

  // Set up loop detection.
  bool loopDetect = false;

  // Run until program counter is equal to or greater than the opcode count.
  while (programCounter < bootCode->count) {
    // Check if we have already hit this opcode.
    if (bootCode->ops[programCounter]->runCount >= 1) {
      loopDetect = true;
      break;
    }

    // Increment runCount for the current opcode.
    bootCode->ops[programCounter]->runCount++;

    // Check what opcode this is.
    if (strcmp(bootCode->ops[programCounter]->op, "acc") == 0) {
      // This is an "acc", so add its arg to the accumulator value.
      *accumulator += bootCode->ops[programCounter]->arg;
      // Increment programCounter by 1;
      programCounter++;
    }
    else if (strcmp(bootCode->ops[programCounter]->op, "jmp") == 0) {
      // This is a "jmp" so add its arg to the programCounter.
      programCounter += bootCode->ops[programCounter]->arg;
    }
    else if (strcmp(bootCode->ops[programCounter]->op, "nop") == 0) {
      // This is a "nop". Does nothing, so just increment programCounter.
      programCounter++;
    }
  }

  // Reset the runCount for all opcodes in the list.
  resetRunCounts(bootCode);

  // Return loopDetect value
  return loopDetect;
}

/**
 * Reset the runCount value for all opcodes in the code list.
 *
 * @param BootCode *bootCode
 *   The code list to reset.
 */
void resetRunCounts(BootCode *bootCode) {
  // @ToDo: Could try to figure out how to do a deep copy in C here instead, but
  // this is way easier to do and is good enough for our purposes.
  for (int i = 0; i < bootCode->count; i++) {
    bootCode->ops[i]->runCount = 0;
  }
}

/**
 * Initialize a bootcode list to a specific size.
 *
 * @param BootCode *bootCode
 *   The bootCode struct to initialize.
 * @param const int size
 *   The size (or number of elements) to initialize this list to.
 */
void init_boot_code(BootCode *bootCode, const int size) {
  // Init count to 0.
  bootCode->count = 0;
  // Init size to passed in size.
  bootCode->size = size;
  // Allocate memory for specified number of Opc pointers.
  bootCode->ops = calloc((size_t) size, sizeof(Opc *));
}
/**
 * Free all memory used by the bootCode list.
 *
 * I am still not completely confident I understand how to properly manage
 * memory, so I am probably leaking memory all over the place. ¯\_(ツ)_/¯
 *
 * @param BootCode *bootCode
 *   The bootCode struct to free.
 */
void free_boot_code(BootCode *bootCode) {
  // If the count is greater than 0, we should have some opcodes to free.
  if (bootCode->count > 0) {
    // Loop over opcodes and free them.
    for (int i = 0; i < bootCode->count; i++) {
      free(bootCode->ops[i]);
    }
  }
  // Free opcode list.
  free(bootCode->ops);
  bootCode->ops = NULL;
}

/**
 * Add an opcode to the opcode list.
 *
 * @param BootCode *bootCode
 *   The opcode list to add an opcode to.
 * @param char *op
 *   The op code.
 * @param long arg
 *   The op arg.
 */
void add_op(BootCode *bootCode, char *op, long arg) {
  // Calculate current load on the bootCode list.
  const int load = bootCode->count * 100 / bootCode->size;
  // If it is more than 70% full, increase the list size.
  if (load > 70) {
    increase_boot_code_array(bootCode);
  }

  // Allocate memory for this opcode.
  bootCode->ops[bootCode->count] = malloc(sizeof(Opc));

  // Insert op code. (This naming is shite. Really should have thought it
  // through a bit more, but now I'm too lazy to go back and fix it).
  bootCode->ops[bootCode->count]->op = strdup(op);
  // Insert op arg.
  bootCode->ops[bootCode->count]->arg = arg;
  // Set runCount to 0;
  bootCode->ops[bootCode->count]->runCount = 0;

  // Increment opcode count.
  bootCode->count++;
}

/**
 * Increase boot code list size.
 *
 * @param BootCode *bootCode
 *   The bootCode list to increase.
 */
void increase_boot_code_array(BootCode *bootCode) {
  // Create a new temp bootCode list.
  BootCode tempBC;
  // Init the temp bootCode list to have double the real lists size.
  init_boot_code(&tempBC, bootCode->size * 2);

  // Loop over real bootCode list and add all of its existing values into the
  // temp bootCode list with doubled size.
  for (int i = 0; i < bootCode->size; i++) {
    Opc *opc = bootCode->ops[i];
    if (opc != NULL) {
      tempBC.ops[i] = opc;
    }
  }

  // Store the real bootCode size value.
  const int tmpSize = bootCode->size;
  // Set the real bootCode size to the temp list size.
  bootCode->size = tempBC.size;
  // Set the real bootCode lists old size into the teml list size.
  tempBC.size = tmpSize;

  // Store the real bootCodes pointer to the old ops list.
  Opc **ops = bootCode->ops;
  // Set the expanded ops list pointer into the real bootCode list.
  bootCode->ops = tempBC.ops;
  // Set the old ops list pointer into the temp list.
  tempBC.ops = ops;

  // Free the temp bootCode list.
  free_boot_code(&tempBC);
}
