#ifndef ir_h
#define ir_h

#include <stdint.h>

#include "token.h"

typedef struct {
} IrAdd;

typedef struct {
	int32_t value;
} IrPush;

typedef enum {
	Ir_Add,
	Ir_Push,
} IrType;

typedef struct Ir Ir;

struct Ir {
	IrType type;
	union {
		IrAdd *add;
		IrPush *push;
	} as;
	Ir *next;
	Ir *previous;
};

Ir *ir_initAdd (void);
Ir *ir_initPush (int32_t value);

#endif
