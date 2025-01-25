#include "ast.h"

struct AstLiteral {
	Token value;
};

struct AstRoot {
	AstLiteral *literal;
};

AstLiteral *ast_initLiteral (Arena *arena, Token value)
{
	AstLiteral *node = arena_allocate(arena, sizeof(*node));
	node->value = value;
	return node;
}

AstRoot *ast_initRoot (Arena *arena, AstLiteral *literal)
{
	AstRoot *node = arena_allocate(arena, sizeof(*node));
	node->literal = literal;
	return node;
}
