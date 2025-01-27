#include "ssa_generator.h"

Ssa *getSsa (Tac *tac);
Tac *getTacEnd (Tac *tac);

typedef struct {
	Arena *arena;
} SsaGenerator;

static SsaGenerator gen;

Ssa *ssa_generate (Arena *arena, Tac *tac)
{
	gen.arena = arena;
	Ssa *ssa;
	for (Tac *c = getTacEnd(tac); c != NULL; c = c->next) {
		Ssa *a = getSsa(c);
		a->next = ssa;
		ssa = a;
	}
	return ssa;
}

Ssa *getSsa (Tac *tac)
{
	switch (tac->type) {
		case Tac_Assign: return ssa_initAssign(gen.arena, tac->as.assign.var, tac->as.assign.value);
		default: return NULL;
	}
}

Tac *getTacEnd (Tac *tac)
{
	Tac *end;
	for (end = tac; end->next != NULL; end = end->next);
	return end;
}
