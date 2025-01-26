#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "tac.h"

typedef enum {
	TacAssign,
} TacType;

struct Tac {
	TacInstruction *head;
};

struct TacInstruction {
	TacType type;
	union {
		struct {
			char *var;
			int32_t value;
		} assign;
	} as;
};

Tac *tac_init (Arena *arena)
{
	Tac *tac = arena_allocate(arena, sizeof(*tac));
	return tac;
}

TacInstruction *tac_init_assign (Arena *arena, char *var, Token value)
{
	TacInstruction *tac = arena_allocate(arena, sizeof(*tac));
	tac->type = TacAssign;
	char *buffer = arena_allocate(arena, value.length + 1);
	sprintf(buffer, "%.*s", value.length, value.lexeme);
	tac->as.assign.var = var;
	tac->as.assign.value = atoi(buffer);
	return tac;
}

void tac_set_head (Tac *tac, TacInstruction *head)
{
	tac->head = head;
}
