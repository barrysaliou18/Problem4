/*
 =====================================================================
 Name        : cpu.h
 Programmers : Joshua Neighbarger | jneigh@uw.edu
               Mamadou Barry | <email>@uw.edu
 Date        : 10 April 2017
 Description : 
 =====================================================================
 */

#ifndef CPU_H
#define CPU_H

#include <assert.h>
#include <stdio.h>

#include "alu.h"
#include "definitions.h"
#include "util.h"

/* ============================ Config ============================ */

#define REGISTER_SIZE 		16
#define TOTAL_REGISTERS 	8
#define TOTAL_RAM 			16

/* ============================ OPCODE ============================ */

#define ADD 				0x0
#define AND 				0x1
#define NOT 				0x2
#define TRAP 				0x3
#define LD  				0x4
#define ST  				0x5
#define JMP 				0x6
#define BR  				0x7

/* ========================== TRAP Codes ========================== */

#define HALT 				0x0
#define GETC 				0x1
#define OUT 				0x2
#define PUTS 				0x3

/* =========================== Bitmasks =========================== */

#define DR_MASK(x) 			(x & 0x0E00)
#define OPCODE_MASK(x) 		(x & 0xF000)
#define SR1_MASK(x) 		(x & 0x01C0)
#define SR2_MASK(x) 		(x & 0x0003)
#define IMMED5_MASK(x) 		(x & 0x001F)
#define IMMED7_MASK(x) 		(x & 0x007F)
#define IMMED8_MASK(x) 		(x & 0x00FF)
#define IMMED9_MASK(x) 		(x & 0x01FF)
#define SEXT6(x)
#define SEXT8(x)
#define SEXT9(x)
#define SEXT11(x)

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

void controller(CPU_s *cpu, Register *memory);


#endif /* CPU_H */
