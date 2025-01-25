#include "ast.h"

struct Ast {
	Token value;
};

Ast *ast_init (Arena *arena)
{
	return arena_allocate(arena, sizeof(Ast));
}

void ast_setValue (Ast *ast, Token value)
{
	ast->value = value;
}
