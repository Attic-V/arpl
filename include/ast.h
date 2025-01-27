#ifndef ast_h
#define ast_h

#include "arena.h"
#include "token.h"

typedef struct {
	Token value;
} AstExpressionNumber;

typedef enum {
	AstExpression_Number,
} AstExpressionType;

typedef struct {
	AstExpressionType type;
	union {
		AstExpressionNumber *number;
	} as;
} AstExpression;

typedef struct {
	AstExpression *expression;
} AstRoot;

typedef struct {
	AstRoot *root;
} Ast;

Ast *ast_init (Arena *arena, AstRoot *root);
AstRoot *ast_initRoot (Arena *arena, AstExpression *expression);
AstExpression *ast_initExpressionNumber (Arena *arena, Token value);

#endif
