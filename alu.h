/*
 =====================================================================
 Name        : alu.h
 Programmers : Joshua Neighbarger | jneigh@uw.edu
               John Bako | jsbako90@uw.edu
 Date        : 10 March 2017
 Description : Performs various operations within the CPU.
 =====================================================================
 */

#ifndef ALU_H
#define ALU_H

#include "definitions.h"
#include "util.h"

/* The Nth bit starts at 1 and goes to 16. */
#define ALU_N_MASK(x) (BIT_MASK_N(x, 1)) // alu->flags |= (n & BIT_MASK_1) << 0;
#define ALU_Z_MASK(x) (BIT_MASK_N(x, 2)) // alu->flags |= (z & BIT_MASK_1) << 1;
#define ALU_P_MASK(x) (BIT_MASK_N(x, 3)) // alu->flags |= (p & BIT_MASK_1) << 2;
#define ALU_C_MASK(x) (BIT_MASK_N(x, 4)) // alu->flags |= (c & BIT_MASK_1) << 3;
#define ALU_O_MASK(x) (BIT_MASK_N(x, 5)) // alu->flags |= (o & BIT_MASK_1) << 4;

typedef Register (*ALUFunction)(Register a, Register b);

typedef struct alu_s {
    ALUFunction op;
    Register a, b, r, flags;
} ALU_s;

void alu_run(ALU_s *alu);
Register add(const Register a, const Register b);
Register subtract(const Register a, const Register b);
Register and(const Register a, const Register b);
Register not(const Register a, const Register b);
Register nand(const Register a, const Register b);

#endif // ALU_H