/*
 =====================================================================
 Name        : cpu.h
 Programmers : Joshua Neighbarger | jneigh@uw.edu
               Mamadou Barry | msbry92@uw.edu
 Date        : 10 April 2017
 Description : 
 =====================================================================
 */

#ifndef CPU_H
#define CPU_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "alu.h"
#include "definitions.h"
#include "util.h"

/* ============================ Config ============================ */

#define REGISTER_SIZE 		16
#define TOTAL_REGISTERS 	8
#define TOTAL_RAM 			24

/* ============================ OPCODE ============================ */

#define BR  				0x0
#define ADD 				0x1
#define LD  				0x2
#define ST  				0x3
#define	JSR					0x4 //JSRR
#define AND 				0x5
#define LDR					0x6
#define STR					0x7
#define RTI					0x8
#define NOT 				0x9
#define LDI					0xA
#define STI					0xB
#define JMP 				0xC // RET
#define RESERVED			0xD
#define	LEA					0xE
#define TRAP 				0xF

/* ========================== TRAP Codes ========================== */

#define GETC 				0x20
#define OUT 				0x21
#define PUTS 				0x22
#define IN					0x23
#define PUTSP				0x24
#define HALT 				0x25

/* =========================== Bitmasks =========================== */

#define DR_MASK(x) 			((x & 0x0E00) >> 9)
#define OPCODE_MASK(x) 		((x & 0xF000) >> 12)
#define SR1_MASK(x) 		(x & 0x01C0)
#define SR2_MASK(x) 		(x & 0x0003)
#define IMMED5_MASK(x) 		(x & 0x001F)
#define IMMED7_MASK(x) 		(x & 0x007F)
#define IMMED8_MASK(x) 		(x & 0x00FF)
#define IMMED9_MASK(x) 		(x & 0x01FF)
#define SEXT5(x)			(BIT_MASK_N(x, 5) ? x | 0xFFE0 : x & BIT_MASK_5)
#define SEXT6(x)			(BIT_MASK_N(x, 6) ? x | 0xF800 : x & BIT_MASK_6)
#define SEXT8(x)			(BIT_MASK_N(x, 8) ? x | 0xFF00 : x & BIT_MASK_8)
#define SEXT9(x)			(BIT_MASK_N(x, 9) ? x | 0xFE00 : x & BIT_MASK_9)
#define SEXT11(x)			(BIT_MASK_N(x, 11) ? x | 0xFFE0 : x & BIT_MASK_11)

/* ======================= Type Definitions ======================= */

typedef enum {
	FETCH,
	DECODE,
	EVAL_ADDR,
	FETCH_OP,
	EXECUTE,
	STORE
} FSU_State;

typedef struct cpu_s {
	Register reg_file[TOTAL_REGISTERS];
	Register ir, pc, mar, mdr;
    unsigned char flags;
    ALU_s *alu;
} CPU_s;

/* ========================== Prototypes ========================== */

void controller(CPU_s *cpu, Register *memory, bool step);
void trap(const unsigned int code, CPU_s *cpu);

#endif /* CPU_H */
