/*
 =====================================================================
 Name        : cpu.c
 Programmers : Joshua Neighbarger | jneigh@uw.edu
               Mamadou Barry | msbry92@uw.edu
 Date        : 10 April 2017
 Description : 

	Instruction Set:
		0001|DR_|SR1|000SR2    ADD     DR <- SR1 + SR2
		0001|DR_|SR1|1_imm5    ADD     DR <- SR1 + sext(imm5)
		0101|DR_|SR1|000SR2    AND     DR <- SR1 & SR2
		0101|DR_|SR1|1_imm5    AND     DR <- SR1 & sext(imm5)
		1001|DR_|SR_|111111    NOT     DR <- ~(SR)
		1111|0000|trapvect8    TRAP    DR <- trap(trapvect8)
		0010|DR_|_PCoffset9    LD      DR <- M[PC + sext(PCoffset9)]
		0011|SR_|_PCoffset9    ST      M[PC + sext(PCoffset9)] <- SR 
		1100|000|BsR|000000    JMP     PC <- BaseR
		0000|nzp|_PCoffset9    BR      PC <- PC + sext(PCOffset9)

 =====================================================================
 */

#include "cpu.h"

void controller(CPU_s *cpu, Register *memory, bool step) {
	unsigned int opcode, DR, SR1, SR2, immed5, immed8, immed9;
	FSU_State state = FETCH;
	assert(memory != NULL);
	assert(cpu != NULL);
	for (;;) {
		switch (state) {
			case FETCH:
				if (step) {
					if (cpu->pc != 0) return;
				}
				printf("Here in FETCH.\n");
				cpu->ir = memory[cpu->pc];
				cpu->pc++;
				printf("\tContents of IR = %04X\n", cpu->ir);
				state = DECODE;
				break;
			case DECODE:
				printf("Here in DECODE.\n");
				opcode = OPCODE_MASK(cpu->ir);
				printf("\tOPCODE = %d\n", opcode);
				switch (opcode) {
					case ADD:
						DR = DR_MASK(cpu->ir);
						SR1 = SR1_MASK(cpu->ir);
						SR2 = SR2_MASK(cpu->ir);
						immed5 = IMMED5_MASK(cpu->ir);
						break;
					case AND:
						DR = DR_MASK(cpu->ir);
						SR1 = SR1_MASK(cpu->ir);
						SR2 = SR2_MASK(cpu->ir);
						immed5 = IMMED5_MASK(cpu->ir);
						break;
					case NOT:
						DR = DR_MASK(cpu->ir);
						SR1 = SR1_MASK(cpu->ir);
						break;
					case TRAP:
						immed8 = IMMED8_MASK(cpu->ir);
						break;
					case LD:
						DR = DR_MASK(cpu->ir);
						immed9 = IMMED9_MASK(cpu->ir);
						break;
					case ST:
						SR1 = DR_MASK(cpu->ir);
						immed9 = IMMED9_MASK(cpu->ir);
						break;
					case JMP:
						SR1 = SR1_MASK(cpu->ir);
						break;
					case BR:
						DR = DR_MASK(cpu->ir); // nzp
						immed9 = IMMED9_MASK(cpu->ir);
						break;
				}
				state = EVAL_ADDR;
				break;
			case EVAL_ADDR:
				printf("Here in EVAL_ADDR.\n");
				switch (opcode) {
					case LD: case ST:
						cpu->mar = cpu->pc + SEXT9(immed9);
						break;
					case JMP:
						cpu->mar = cpu->reg_file[SR1];
						break;
					default:
						break;
				}
				state = FETCH_OP;
				break;
			case FETCH_OP:
				printf("Here in FETCH_OP.\n");
				switch (opcode) {
					case ADD:
						cpu->alu->a = cpu->reg_file[SR1];
						cpu->alu->b = BIT_MASK_N(cpu->ir, 5) ? cpu->reg_file[SR2] : SEXT5(immed5);
						cpu->alu->op = add;
					case AND:
						cpu->alu->a = cpu->reg_file[SR1];
						cpu->alu->b = BIT_MASK_N(cpu->ir, 5) ? cpu->reg_file[SR2] : SEXT5(immed5);
						cpu->alu->op = and;
						break;
					case NOT:
						cpu->alu->a = cpu->reg_file[SR1];
						cpu->alu->b = cpu->reg_file[SR2];
						cpu->alu->op = not;
						break;
					case LD:
						cpu->mdr = memory[cpu->mar];
						break;
					case ST:
						cpu->mdr = cpu->reg_file[SR1];
					case JMP:
						cpu->mdr = memory[cpu->mar];
						break;
					case BR:
						break;
				}
				state = EXECUTE;
				break;
			case EXECUTE:
				printf("Here in EXECUTE.\n");
				switch (opcode) {
					case TRAP:
						trap(immed8, cpu);
						break;
					case BR:
						break;
					default:
						alu_run(cpu->alu);
						break;
				}
				state = STORE;
				break;
			case STORE:
				printf("Here in STORE.\n");
				switch (opcode) {
					case ADD:
						break;
					case AND:
						break;
					case NOT:
						break;
					case TRAP:
						break;
					case LD:
						cpu->reg_file[DR] = cpu->mdr;
						break;
					case ST:
						memory[cpu->mar] = cpu->mdr;
						break;
					case JMP:
						break;
					case BR:
						break;
					default:
						cpu->reg_file[DR] = cpu->alu->r;
						break;
				}
				state = FETCH;
				break;
		}
	}
}

void trap(const unsigned int code, CPU_s *cpu) {
	switch (code) {
		case GETC:
			break;
		case OUT:
			break;
		case PUTS:
			break;
		case IN:
			break;
		case PUTSP:
			break;
		case HALT:
			exit(0);
			break;
		default:
			break;
	}
}
