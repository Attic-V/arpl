#ifndef tac_h
#define tac_h

#include <stdint.h>

#include "arena.h"
#include "token.h"

typedef struct {
	char *var;
	int32_t value;
} TacAssign;

typedef enum {
	Tac_Assign,
} TacType;

typedef struct Tac Tac;

struct Tac {
	TacType type;
	union {
		TacAssign assign;
	} as;
	Tac *next;
	Tac *previous;
};

Tac *tac_initAssign (Arena *arena, char *var, Token value);

#endif
