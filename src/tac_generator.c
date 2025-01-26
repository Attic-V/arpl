#include "tac_generator.h"

Tac *visitAst (Ast *ast);
TacInstruction *visitRoot (AstRoot *root);
TacInstruction *visitLiteral (AstLiteral *literal);

typedef struct {
	Arena *arena;
} Generator;

Generator generator;

Tac *tac_generate (Arena *arena, Ast *ast)
{
	generator.arena = arena;
	return visitAst(ast);
}

Tac *visitAst (Ast *ast)
{
	Tac *tac = tac_init(generator.arena);
	TacInstruction *next = visitRoot(ast->root);
	tac_set_head(tac, next);
	return tac;
}

TacInstruction *visitRoot (AstRoot *root)
{
	return visitLiteral(root->literal);
}

TacInstruction *visitLiteral (AstLiteral *literal)
{
	TacInstruction *ins = tac_init_const(generator.arena, literal->value);
	return ins;
}
