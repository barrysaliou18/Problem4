; FSMTest.asm
; TCSS 372, April 2017
; Joshua Neighbarger | jneigh@uw.edu
; Mamadou Barry | msbry92@uw.edu

	.ORIG x3000

	AND	R0, R0, #0
	AND	R1, R1, #0
	AND	R2, R2, #0
	AND	R3, R3, #0
	AND	R4, R4, #0
	AND	R5, R5, #0
	AND	R6, R6, #0
	AND	R7, R7, #0

	ADD	R1, R1, #10
	ADD	R2, R2, #-5
	ADD	R3, R1, R2
	ST	R3, RESULT

	NOT	R2, R2
	LD	R3, NEG_FIVE
	ADD	R0, R1, R3

	HALT

NEG_FIVE	.FILL	xFFFA
RESULT	.BLKW	1
	.END
