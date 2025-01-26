#include "tac_generator.h"

typedef struct {
	Arena *arena;
} Generator;

Generator generator;

Tac *tac_generate (Arena *arena, Ast *ast)
{
	generator.arena = arena;
	Tac *tac = tac_init(arena);
	TacInstruction *next = tac_init_const(arena, ast->root->literal->value);
	tac_set_head(tac, next);
	return tac;
}
