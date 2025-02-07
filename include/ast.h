#ifndef ast_h
#define ast_h

#include "data_type.h"
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

typedef struct {
	Token operator;
	AstExpression *right;
} AstExpressionUnary;

typedef enum {
	AstExpression_Binary,
	AstExpression_Number,
	AstExpression_Unary,
} AstExpressionType;

struct AstExpression {
	AstExpressionType type;
	union {
		AstExpressionBinary *binary;
		AstExpressionNumber *number;
		AstExpressionUnary *unary;
	} as;
	DataType dataType;
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
AstExpression *ast_initExpressionUnary (Token operator, AstExpression *right);

#endif
