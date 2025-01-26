#include <math.h>
#include <stdio.h>
#include <string.h>

#include "tac_generator.h"

Tac *visitAst (Ast *ast);
TacInstruction *visitRoot (AstRoot *root);
TacInstruction *visitLiteral (AstLiteral *literal);

typedef struct {
	Arena *arena;
	int temp;
} Generator;

Generator generator;

Tac *tac_generate (Arena *arena, Ast *ast)
{
	generator.arena = arena;
	generator.temp = 0;
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
	char *buffer = arena_allocate(generator.arena, (int)log10f(generator.temp ? generator.temp : 1) + 2);
	sprintf(buffer, "t%d", generator.temp++);
	return tac_init_assign(generator.arena, buffer, literal->value);
}
