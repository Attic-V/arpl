#ifndef ir_h
#define ir_h

#include <stdint.h>

#include "token.h"

typedef struct {
} IrAdd;

typedef struct {
} IrMul;

typedef struct {
} IrNeg;

typedef struct {
	int32_t value;
} IrPush;

typedef struct {
} IrSub;

typedef enum {
	Ir_Add,
	Ir_Mul,
	Ir_Neg,
	Ir_Push,
	Ir_Sub,
} IrType;

typedef struct Ir Ir;

struct Ir {
	IrType type;
	union {
		IrAdd *add;
		IrMul *mul;
		IrNeg *neg;
		IrPush *push;
		IrSub *sub;
	} as;
	Ir *next;
	Ir *previous;
};

Ir *ir_initAdd (void);
Ir *ir_initMul (void);
Ir *ir_initNeg (void);
Ir *ir_initPush (int32_t value);
Ir *ir_initSub (void);

#endif
