#ifndef ir_h
#define ir_h

#include <stdint.h>

#include "token.h"

typedef struct {
	size_t size;
} IrAdd;

typedef struct {
	size_t size;
} IrAnd;

typedef struct {
	size_t size;
} IrAssign;

typedef struct {
} IrCopy;

typedef struct {
	size_t size;
} IrDec;

typedef struct {
	size_t size;
} IrDeref;

typedef struct {
	size_t size;
} IrEqu;

typedef struct {
	size_t size;
} IrInc;

typedef struct {
	int n;
} IrJmp;

typedef struct {
	int n;
} IrJmpFalse;

typedef struct {
	int n;
} IrJmpTrue;

typedef struct {
	int n;
} IrLabel;

typedef struct {
	size_t size;
} IrLess;

typedef struct {
	size_t size;
} IrLessEqu;

typedef struct {
	size_t size;
} IrMul;

typedef struct {
	size_t size;
} IrNeg;

typedef struct {
	size_t size;
} IrNot;

typedef struct {
	size_t size;
} IrNotEqu;

typedef struct {
	size_t size;
} IrOr;

typedef struct {
} IrPop;

typedef struct {
	int32_t value;
} IrPush;

typedef struct {
	size_t idx;
} IrRef;

typedef struct {
	size_t bytes;
} IrReserve;

typedef struct {
} IrRet;

typedef struct {
	size_t size;
} IrSar;

typedef struct {
	size_t size;
} IrShl;

typedef struct {
	size_t size;
} IrSub;

typedef struct {
	size_t idx;
	size_t size;
} IrVal;

typedef struct {
	size_t size;
} IrXor;

typedef enum {
	Ir_Add,
	Ir_And,
	Ir_Assign,
	Ir_Copy,
	Ir_Dec,
	Ir_Deref,
	Ir_Equ,
	Ir_Inc,
	Ir_Jmp,
	Ir_JmpFalse,
	Ir_JmpTrue,
	Ir_Label,
	Ir_Less,
	Ir_LessEqu,
	Ir_Mul,
	Ir_Neg,
	Ir_Not,
	Ir_NotEqu,
	Ir_Or,
	Ir_Pop,
	Ir_Push,
	Ir_Ref,
	Ir_Reserve,
	Ir_Ret,
	Ir_Sar,
	Ir_Shl,
	Ir_Sub,
	Ir_Val,
	Ir_Xor,
} IrType;

typedef struct Ir Ir;

struct Ir {
	IrType type;
	union {
		IrAdd *add;
		IrAnd *and;
		IrAssign *assign;
		IrCopy *copy;
		IrDec *dec;
		IrDeref *deref;
		IrEqu *equ;
		IrInc *inc;
		IrJmp *jmp;
		IrJmpFalse *jmpFalse;
		IrJmpTrue *jmpTrue;
		IrLabel *label;
		IrLess *less;
		IrLessEqu *lessEqu;
		IrMul *mul;
		IrNeg *neg;
		IrNot *not;
		IrNotEqu *notEqu;
		IrOr *or;
		IrPop *pop;
		IrPush *push;
		IrRef *ref;
		IrReserve *reserve;
		IrRet *ret;
		IrSar *sar;
		IrShl *shl;
		IrSub *sub;
		IrVal *val;
		IrXor *xor;
	} as;
	Ir *next;
	Ir *previous;
};

Ir *ir_initAdd (size_t size);
Ir *ir_initAnd (size_t size);
Ir *ir_initAssign (size_t size);
Ir *ir_initCopy (void);
Ir *ir_initDec (size_t size);
Ir *ir_initDeref (size_t size);
Ir *ir_initEqu (size_t size);
Ir *ir_initInc (size_t size);
Ir *ir_initJmp (int n);
Ir *ir_initJmpFalse (int n);
Ir *ir_initJmpTrue (int n);
Ir *ir_initLabel (int n);
Ir *ir_initLess (size_t size);
Ir *ir_initLessEqu (size_t size);
Ir *ir_initMul (size_t size);
Ir *ir_initNeg (size_t size);
Ir *ir_initNot (size_t size);
Ir *ir_initNotEqu (size_t size);
Ir *ir_initOr (size_t size);
Ir *ir_initPop (void);
Ir *ir_initPush (int32_t value);
Ir *ir_initRef (size_t idx);
Ir *ir_initReserve (size_t bytes);
Ir *ir_initRet (void);
Ir *ir_initSar (size_t size);
Ir *ir_initShl (size_t size);
Ir *ir_initSub (size_t size);
Ir *ir_initVal (size_t idx, size_t size);
Ir *ir_initXor (size_t size);

#endif
