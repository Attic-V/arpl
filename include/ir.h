#ifndef ir_h
#define ir_h

#include <stdint.h>

#include "token.h"

typedef struct {
} IrAdd;

typedef struct {
} IrAnd;

typedef struct {
} IrEqu;

typedef struct {
} IrMul;

typedef struct {
} IrNeg;

typedef struct {
} IrNot;

typedef struct {
} IrNotEqu;

typedef struct {
} IrOr;

typedef struct {
	int32_t value;
} IrPush;

typedef struct {
} IrSub;

typedef enum {
	Ir_Add,
	Ir_And,
	Ir_Equ,
	Ir_Mul,
	Ir_Neg,
	Ir_Not,
	Ir_NotEqu,
	Ir_Or,
	Ir_Push,
	Ir_Sub,
} IrType;

typedef struct Ir Ir;

struct Ir {
	IrType type;
	union {
		IrAdd *add;
		IrAnd *and;
		IrEqu *equ;
		IrMul *mul;
		IrNeg *neg;
		IrNot *not;
		IrNotEqu *notEqu;
		IrOr *or;
		IrPush *push;
		IrSub *sub;
	} as;
	Ir *next;
	Ir *previous;
};

Ir *ir_initAdd (void);
Ir *ir_initAnd (void);
Ir *ir_initEqu (void);
Ir *ir_initMul (void);
Ir *ir_initNeg (void);
Ir *ir_initNot (void);
Ir *ir_initOr (void);
Ir *ir_initNotEqu (void);
Ir *ir_initPush (int32_t value);
Ir *ir_initSub (void);

#endif
