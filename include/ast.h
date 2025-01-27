#ifndef ast_h
#define ast_h

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

Ast *ast_init (AstRoot *root);
AstRoot *ast_initRoot (AstExpression *expression);
AstExpression *ast_initExpressionNumber (Token value);

#endif
