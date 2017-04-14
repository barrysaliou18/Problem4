/*
 =====================================================================
 Name        : lc3.c
 Programmers : Joshua Neighbarger | jneigh@uw.edu
               Mamadou Barry | msbry92@uw.edu
 Date        : 10 April 2017
 Description : 
 =====================================================================
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cpu.h"

/* Amount of RAM is defined as the number of words in Memory (double 
 * the value to get number of Bytes) */
#define NUM_WORDS_TO_DISPLAY 16
#define MEMORY_STARTING_ADDR 0x3000
#define INVALID_SELECTION "\nInvalid input. Please select a number between %d and %d.\n"

static void clearScreen();
static void printDebug(CPU_s *cpu, Register *memory);
static void load(Register *memory);
static void run(CPU_s *cpu, Register *memory);
static void clearMem(Register *memory);
static void showMem();

int main(const int argc, const char *argv[]) {
	int status = 0, selection = 0;
	CPU_s *cpu = calloc(sizeof(CPU_s), 1);
	cpu->alu = calloc(sizeof(ALU_s), 1);
	Register memory[TOTAL_RAM];

	assert(cpu != NULL);
	assert(cpu->alu != NULL);
	assert(memory != NULL);

	clearMem(memory);
	setvbuf(stdout, NULL, _IONBF, 0);
	if (argc == 2 && argv[1][0] == '0' && argv[1][1] == 'x') {
		Register ir;
		sscanf(argv[1], "%X", &ir);
		memory[0] = ir;
		printDebug(cpu, memory);
		controller(cpu, memory, true);
		printDebug(cpu, memory);
	} else if (argc == 2 && !strcmp(argv[1], "-gui")) {
		do {
			selection = 0;
			clearScreen();
			printDebug(cpu, memory);
			printf("1) Load, 2) Run, 3) Clear Mem, 4) Show Mem, ");
			printf("5) Exit\n::_");
			scanf("%d", &selection);
			switch(selection) {
				case 1:
					load(memory);
					break;
				case 2:
					run(cpu, memory);
					break;
				case 3:
					clearMem(memory);
					break;
				case 4:
					showMem(memory);
					break;
				case 5:
					break;
				default:
					printf(INVALID_SELECTION, 1, 5);
					sleep(2);
					break;
			}
		} while (selection != 5);
	} else {
		fprintf(stderr, "argc: %d, argv[1][0]: %c, argv[1][1]: %c\n", argc, argv[1][0], argv[1][1]);
		fprintf(stderr, "Usage: lc3 {-gui|0x????} \n");
		fprintf(stderr, "-gui Launches the program in GUI mode (experimental) \n");
		fprintf(stderr, "0x???? Runs the defined 16-bit IR in HEX \n");
	}

	if (cpu->alu != NULL) free(cpu->alu);
	if (cpu != NULL) free(cpu);
	return status;
}

/*
 * clearScreen
 * -----------
 * Clears the console.
 */

static void clearScreen() {
	for (int i = 0; i < 50; i++) printf("\n");
}

/*
 * printDebug
 * ----------
 * Prints the debug output of the computer's state.
 */

static void printDebug(CPU_s *cpu, Register *memory) {
	unsigned int i = 0;
	printf("%40s\n", "Memory");
	for (i = 0; i < TOTAL_RAM - TOTAL_REGISTERS - 8; i++)
		printf("\t\t\t\t  0x%04X: 0x%04X\n", i + MEMORY_STARTING_ADDR, memory[i]);
	printf("Registers%25s0x%04X: 0x%04X\n", "", i + MEMORY_STARTING_ADDR, memory[i]); i++;
	for (int j = 0; j < TOTAL_REGISTERS; i++, j++)
		printf("R%d: 0x%04X                        0x%04X: 0x%04X\n", j, cpu->reg_file[j], i + MEMORY_STARTING_ADDR, memory[i]);
	printf("%34s0x%04X: 0x%04X\n", "", i + MEMORY_STARTING_ADDR, memory[i]); i++;
	printf("%34s0x%04X: 0x%04X\n", "", i + MEMORY_STARTING_ADDR, memory[i]); i++;
	printf("Special Registers%17s0x%04X: 0x%04X\n", "", i + MEMORY_STARTING_ADDR, memory[i]); i++;
	printf("PC:  0x%04X    IR:  0x%04X        0x%04X: 0x%04X\n", cpu->pc, cpu->ir, i + MEMORY_STARTING_ADDR, memory[i]); i++; 			// Next instruction address, current instruction
	printf("MAR: 0x%04X    MDR: 0x%04X        0x%04X: 0x%04X\n", cpu->mar, cpu->mdr, i + MEMORY_STARTING_ADDR, memory[i]); i++;		// Address of memory, Value of memory
	printf("Status Flags%22s0x%04X: 0x%04X\n", "", i + MEMORY_STARTING_ADDR, memory[i]); i++;
	printf("N: %d, Z: %d, P: %d, C: %d, O: %d      0x%04X: 0x%04X\n\n",
		ALU_N_MASK(cpu->alu->flags), ALU_Z_MASK(cpu->alu->flags), 
		ALU_P_MASK(cpu->alu->flags), ALU_C_MASK(cpu->alu->flags), 
		ALU_O_MASK(cpu->alu->flags), i + MEMORY_STARTING_ADDR, memory[i]);
}

/*
 * load
 * ----
 * Prompts the user for a filename and tries to load the hex program into memory.
 */

static void load(Register *memory) {
	char *path = calloc(sizeof(char), 256);
	printf("Which file would you like to load?\n::_");
	scanf("%s", path);
	FILE *src = fopen(path, "r");
	for (int i = 0, var = 0; i < TOTAL_RAM 
		&& fscanf(src, "%04X", &var) != EOF; i++)
		memory[i] = var & BIT_MASK_16;
	fclose(src);
}

/*
 * run
 * ---
 * Runs the program that is currently in memory.
 */

static void run(CPU_s *cpu, Register *memory) {
	int selection = 0;
	printf("1) Run, 2) Step\n::_");
	scanf("%d", &selection);
	if (selection == 1 || selection == 2) controller(cpu, memory, false);
	else {
		printf(INVALID_SELECTION, 1, 2);
		sleep(2);
		return;
	}
}

/*
 * clearMem
 * --------
 * Clears memory.
 */

static void clearMem(Register *memory) {
	for (int i = 0; i < TOTAL_RAM; i++)
		memory[i] = 0 & BIT_MASK_16;
}

/*
 * showMem
 * -------
 * Shows the next NUM_WORDS_TO_DISPLAY words of memory after the 
 * prompted address if they exist in memory.
 */

static void showMem(Register *memory) {
	int i;
	printf("Enter the starting address:\n::_");
	scanf("%X", &i);
	printf("\n");
	for (int j = 0; j < NUM_WORDS_TO_DISPLAY && i < TOTAL_RAM; j++, i++)
		printf("0x%04X: 0x%04X\n", i, memory[i]);
	sleep(2);
}
