#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "tac.h"

struct Tac {
	TacInstruction *head;
};

struct TacInstruction {
	int32_t value;
};

Tac *tac_init (Arena *arena)
{
	Tac *tac = arena_allocate(arena, sizeof(*tac));
	return tac;
}

TacInstruction *tac_init_const (Arena *arena, Token value)
{
	TacInstruction *tac = arena_allocate(arena, sizeof(*tac));
	char *buffer = arena_allocate(arena, value.length + 1);
	sprintf(buffer, "%.*s", value.length, value.lexeme);
	tac->value = atoi(buffer);
	return tac;
}

void tac_set_head (Tac *tac, TacInstruction *head)
{
	tac->head = head;
}
