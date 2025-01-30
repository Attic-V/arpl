#ifndef ast_h
#define ast_h

#include "token.h"

typedef struct AstExpression AstExpression;

typedef struct {
	AstExpression *a;
	AstExpression *b;
	Token operator;
} AstExpressionBinary;

typedef struct {
	Token value;
} AstExpressionNumber;

typedef enum {
	AstExpression_Binary,
	AstExpression_Number,
} AstExpressionType;

struct AstExpression {
	AstExpressionType type;
	union {
		AstExpressionBinary *binary;
		AstExpressionNumber *number;
	} as;
};

typedef struct {
	AstExpression *expression;
} AstRoot;

typedef struct {
	AstRoot *root;
} Ast;

Ast *ast_init (AstRoot *root);
AstRoot *ast_initRoot (AstExpression *expression);
AstExpression *ast_initExpressionBinary (AstExpression *a, AstExpression *b, Token operator);
AstExpression *ast_initExpressionNumber (Token value);

#endif
