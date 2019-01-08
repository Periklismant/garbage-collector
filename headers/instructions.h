#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#define HALT   0
#define JUMP   1
#define JNZ    2
#define DUP    3
#define SWAP   4
#define DROP   5
#define PUSH4  6
#define PUSH2  7
#define PUSH1  8
#define ADD    9
#define SUB    10
#define MUL    11
#define DIV    12
#define MOD    13
#define EQ     14
#define NE     15
#define LT     16
#define GT     17
#define LE     18
#define GE     19
#define NOT    20
#define AND    21
#define OR     22
#define INPUT  23
#define OUTPUT 24
#define CLOCK  42
#define CONS   48
#define HD     49
#define TL     50

#define HALT_SIZEOF   1
#define JUMP_SIZEOF   3
#define JNZ_SIZEOF    3
#define DUP_SIZEOF    2
#define SWAP_SIZEOF   2
#define DROP_SIZEOF   1
#define PUSH4_SIZEOF  5
#define PUSH2_SIZEOF  3
#define PUSH1_SIZEOF  2
#define ADD_SIZEOF    1
#define SUB_SIZEOF    1
#define MUL_SIZEOF    1
#define DIV_SIZEOF    1
#define MOD_SIZEOF    1
#define EQ_SIZEOF     1
#define NE_SIZEOF     1
#define LT_SIZEOF     1
#define GT_SIZEOF     1
#define LE_SIZEOF     1
#define GE_SIZEOF     1
#define NOT_SIZEOF    1
#define AND_SIZEOF    1
#define OR_SIZEOF     1
#define INPUT_SIZEOF  1
#define OUTPUT_SIZEOF 1
#define CLOCK_SIZEOF  1
#define CONS_SIZEOF   1
#define HD_SIZEOF     1
#define TL_SIZEOF     1

#endif
