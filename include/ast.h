#ifndef ast_h
#define ast_h

#include <stdbool.h>

#include "data_type.h"
#include "token.h"

typedef struct AstExpression AstExpression;

typedef struct {
	AstExpression *a;
	AstExpression *b;
	Token operator;
} AstExpressionBinary;

typedef struct {
	bool value;
} AstExpressionBoolean;

typedef struct {
	Token value;
} AstExpressionNumber;

typedef struct {
	AstExpression *condition;
	AstExpression *a;
	AstExpression *b;
	Token operator;
} AstExpressionTernary;

typedef struct {
	Token operator;
	AstExpression *right;
} AstExpressionUnary;

typedef enum {
	AstExpression_Binary,
	AstExpression_Boolean,
	AstExpression_Number,
	AstExpression_Ternary,
	AstExpression_Unary,
} AstExpressionType;

struct AstExpression {
	AstExpressionType type;
	union {
		AstExpressionBinary *binary;
		AstExpressionBoolean *boolean;
		AstExpressionNumber *number;
		AstExpressionTernary *ternary;
		AstExpressionUnary *unary;
	} as;
	DataType dataType;
};

typedef struct AstStatement AstStatement;

typedef struct {
	AstStatement *children;
} AstStatementBlock;

typedef struct {
	AstExpression *expression;
} AstStatementExpr;

typedef enum {
	AstStatement_Block,
	AstStatement_Expr,
} AstStatementType;

struct AstStatement {
	AstStatementType type;
	union {
		AstStatementBlock *block;
		AstStatementExpr *expr;
	} as;
	AstStatement* next;
	AstStatement* previous;
};

typedef struct {
	AstStatement *statement;
} AstRoot;

typedef struct {
	AstRoot *root;
} Ast;

Ast *ast_init (AstRoot *root);
AstRoot *ast_initRoot (AstStatement *statement);

AstStatement *ast_initStatementBlock (AstStatement *children);
AstStatement *ast_initStatementExpr (AstExpression *expression);

AstExpression *ast_initExpressionBinary (AstExpression *a, AstExpression *b, Token operator);
AstExpression *ast_initExpressionBoolean (bool value);
AstExpression *ast_initExpressionNumber (Token value);
AstExpression *ast_initExpressionTernary (AstExpression *condition, AstExpression *a, AstExpression *b, Token operator);
AstExpression *ast_initExpressionUnary (Token operator, AstExpression *right);

#endif
