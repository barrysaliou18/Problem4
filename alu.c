/*
 =====================================================================
 Name        : alu.c
 Programmers : Joshua Neighbarger | jneigh@uw.edu
               Mamadou Barry | msbry92@uw.edu
 Date        : 10 April 2017
 Description : 
 =====================================================================
 */

#include "alu.h"

/*
 * alu_run
 * -------
 * Runs the ALU with the set operator and operands.
 */

void alu_run(ALU_s *alu) {
	Bit n = 0, z = 0, p = 0, c = 0, o = 0;
	alu->r = alu->op(alu->a, alu->b);

	// determine the value of each flag

	if (subtract(alu->a, alu->b) && alu->a == alu->b) { // if a = b (z flag is set)
		z = 1;
	} else if (subtract(alu->a, alu->b) && alu->a != alu->b) {
		z = 0;
	} else if (subtract(alu->a, alu->b) && alu->b > alu->a) { // if b > a (c flag is set), if b <= a (c flag is off)
		c = 1;
	} else if (subtract(alu->a, alu->b) && alu->b <= alu->a) {
		c = 0;
	} else if (subtract(alu->a, alu->b) && alu->a < alu->b) { // if a < b, negative (n flag is set), if a >= b (n flag is off)
		n = 1;
	} else if (subtract(alu->a, alu->b) && alu->a >= alu->b) {
		n = 0;
	} else if (add(alu->a >= 0, alu->b >= 0) < 0) { // V=1 when the result of *Pos + Pos is a Neg *Neg + Neg is a Pos
		o = 1;
		p = 0;
	} else if (add(alu->a < 0, alu->b < 0) >= 0) { // V=0 when otherwise
		o = 0;
		p = 1;
	} // do p
	
	alu->flags |= (n & BIT_MASK_1) << 0;
	alu->flags |= (z & BIT_MASK_1) << 1;
	alu->flags |= (p & BIT_MASK_1) << 2;
	alu->flags |= (c & BIT_MASK_1) << 3;
	alu->flags |= (o & BIT_MASK_1) << 4;
}

/************************** ALU Operations **************************/

/*
 * add
 * ---
 * Adds the two passed Registers and returns the resulting value.
 * 
 * a: the first Register to add
 * b: the second Register to add
 */

Register add(const Register a, const Register b) {
	return a + b;
}

/*
 * Subtract
 * ---
 * Subtract the two passed Registers and returns the resulting value.
 *
 * a: the first Register to subtract
 * b: the second Register to subtract
 */

Register subtract(const Register a, const Register b) {
	return a - b;
}

/*
 * and
 * ----
 * ANDs the two passed Registers and returns the resulting value.
 * 
 * a: the first Register to AND
 * b: the second Register to AND
 */

Register and(const Register a, const Register b) {
	return a & b;
}

/*
 * not
 * ----
 * NOTs the two passed Registers and returns the resulting value.
 * 
 * a: the first Register to NOT
 * b: ignored
 */

Register not(const Register a, const Register b) {
	return ~a;
}

/*
 * nand
 * ----
 * NANDs the two passed Registers and returns the resulting value.
 * 
 * a: the first Register to NAND
 * b: the second Register to NAND
 */

Register nand(const Register a, const Register b) {
	return ~(a & b);
}
