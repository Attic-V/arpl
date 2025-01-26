#ifndef ast_h
#define ast_h

#include "arena.h"
#include "token.h"

typedef struct {
	Token value;
} AstLiteral;

typedef struct {
	AstLiteral *literal;
} AstRoot;

typedef struct {
	AstRoot *root;
} Ast;

Ast *ast_init (Arena *arena, AstRoot *root);
AstLiteral *ast_initLiteral (Arena *arena, Token value);
AstRoot *ast_initRoot (Arena *arena, AstLiteral *literal);

#endif
