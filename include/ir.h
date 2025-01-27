#ifndef ir_h
#define ir_h

#include <stdint.h>

#include "token.h"

typedef struct {
	char *var;
	int32_t value;
} IrAssign;

typedef enum {
	Ir_Assign,
} IrType;

typedef struct Ir Ir;

struct Ir {
	IrType type;
	union {
		IrAssign assign;
	} as;
	Ir *next;
	Ir *previous;
};

Ir *ir_initAssign (char *var, int32_t value);

#endif
