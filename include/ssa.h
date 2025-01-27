#ifndef ssa_h
#define ssa_h

#include <stdint.h>

#include "arena.h"

typedef enum {
	Ssa_Assign,
} SsaType;

typedef struct {
	char *var;
	int32_t value;
} SsaAssign;

typedef struct Ssa Ssa;

struct Ssa {
	SsaType type;
	union {
		SsaAssign assign;
	} as;
	Ssa *next;
	Ssa *previous;
};

Ssa *ssa_initAssign (Arena *arena, char *var, int32_t value);

#endif
