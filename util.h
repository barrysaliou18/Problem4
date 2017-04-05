/*
 =====================================================================
 Name        : util.h
 Programmers : Joshua Neighbarger | jneigh@uw.edu
               John Bako | jsbako90@uw.edu
 Date        : 10 March 2017
 Description : Utility header which contains useful MACRO functions.
 =====================================================================
 */

#ifndef UTIL_H
#define UTIL_H

#define BIT_MASK_1 0x1
#define BIT_MASK_2 0x3
#define BIT_MASK_3 0x7
#define BIT_MASK_5 0x1F
#define BIT_MASK_7 0x7F
#define BIT_MASK_9 0x1FF
#define BIT_MASK_10 0x3FF
#define BIT_MASK_16 0xFFFF
#define BIT_MASK_N(a, n) (a >> (n - 1) & BIT_MASK_1)
#define PRINT_BIN_N(i, n) \
	for (int j = n - 1; j >= 0; j--) {printf("%d", BIT_MASK_N(i, j));}

#endif // UTIL_H
