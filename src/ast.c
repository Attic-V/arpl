#include "ast.h"

Ast *ast_init (Arena *arena);

typedef enum {
	NLiteral,
	NRoot,
} AstNodeType;

struct Ast {
	AstNodeType type;
	union {
		struct {
			Token value;
		} literal;
		struct {
			Ast *literal;
		} root;
	} as;
};

Ast *ast_initLiteral (Arena *arena, Token value)
{
	Ast *ast = ast_init(arena);
	ast->type = NLiteral;
	ast->as.literal.value = value;
	return ast;
}

Ast *ast_initRoot (Arena *arena, Ast *literal)
{
	Ast *ast = ast_init(arena);
	ast->type = NRoot;
	ast->as.root.literal = literal;
	return ast;
}

Ast *ast_init (Arena *arena)
{
	return arena_allocate(arena, sizeof(Ast));
}
