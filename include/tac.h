#ifndef tac_h
#define tac_h

#include <stdint.h>

#include "arena.h"
#include "token.h"

typedef enum {
	TacAssign,
} TacType;

typedef struct {
	TacType type;
	union {
		struct {
			char *var;
			int32_t value;
		} assign;
	} as;
} TacInstruction;

typedef struct {
	TacInstruction *head;
} Tac;

Tac *tac_init (Arena *arena);
TacInstruction *tac_init_assign (Arena *arena, char *var, Token value);

void tac_set_head (Tac *tac, TacInstruction *head);

#endif
