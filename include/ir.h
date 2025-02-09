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
	int n;
} IrJmp;

typedef struct {
	int n;
} IrJmpFalse;

typedef struct {
	int n;
} IrLabel;

typedef struct {
} IrLess;

typedef struct {
} IrLessEqu;

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
} IrSar;

typedef struct {
} IrShl;

typedef struct {
} IrSub;

typedef struct {
} IrXor;

typedef enum {
	Ir_Add,
	Ir_And,
	Ir_Equ,
	Ir_Jmp,
	Ir_JmpFalse,
	Ir_Label,
	Ir_Less,
	Ir_LessEqu,
	Ir_Mul,
	Ir_Neg,
	Ir_Not,
	Ir_NotEqu,
	Ir_Or,
	Ir_Push,
	Ir_Sar,
	Ir_Shl,
	Ir_Sub,
	Ir_Xor,
} IrType;

typedef struct Ir Ir;

struct Ir {
	IrType type;
	union {
		IrAdd *add;
		IrAnd *and;
		IrEqu *equ;
		IrJmp *jmp;
		IrJmpFalse *jmpFalse;
		IrLabel *label;
		IrLess *less;
		IrLessEqu *lessEqu;
		IrMul *mul;
		IrNeg *neg;
		IrNot *not;
		IrNotEqu *notEqu;
		IrOr *or;
		IrPush *push;
		IrSar *sar;
		IrShl *shl;
		IrSub *sub;
		IrXor *xor;
	} as;
	Ir *next;
	Ir *previous;
};

Ir *ir_initAdd (void);
Ir *ir_initAnd (void);
Ir *ir_initEqu (void);
Ir *ir_initJmp (int n);
Ir *ir_initJmpFalse (int n);
Ir *ir_initLabel (int n);
Ir *ir_initLess (void);
Ir *ir_initLessEqu (void);
Ir *ir_initMul (void);
Ir *ir_initNeg (void);
Ir *ir_initNot (void);
Ir *ir_initNotEqu (void);
Ir *ir_initOr (void);
Ir *ir_initPush (int32_t value);
Ir *ir_initSar (void);
Ir *ir_initShl (void);
Ir *ir_initSub (void);
Ir *ir_initXor (void);

#endif
