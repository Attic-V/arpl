#include "ast.h"

Ast *ast_init (Arena *arena, AstRoot *root)
{
	Ast *ast = arena_allocate(arena, sizeof(*ast));
	ast->root = root;
	return ast;
}

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
