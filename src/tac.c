#include <stdio.h>
#include <stdlib.h>

#include "tac.h"

Tac *tac_initAssign (Arena *arena, char *var, Token value)
{
	Tac *tac = arena_allocate(arena, sizeof(*tac));
	tac->type = Tac_Assign;
	char *buffer = arena_allocate(arena, value.length + 1);
	sprintf(buffer, "%.*s", value.length, value.lexeme);
	tac->as.assign.var = var;
	tac->as.assign.value = atoi(buffer);
	tac->next = NULL;
	tac->previous = NULL;
	return tac;
}
