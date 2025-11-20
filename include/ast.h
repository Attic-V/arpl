#ifndef ast_h
#define ast_h

#include <stdbool.h>

#include "data_type.h"
#include "scope.h"
#include "table.h"
#include "token.h"

typedef struct AstExpression AstExpression;

typedef struct {
	AstExpression *e;
	Token op;
	Token mToken;
	DataType *mDataType;
} AstExpressionAccess;

typedef struct {
	AstExpression *a;
	AstExpression *b;
	Token operator;
} AstExpressionAssign;

typedef struct {
	AstExpression *a;
	AstExpression *b;
	Token operator;
} AstExpressionBinary;

typedef struct {
	bool value;
} AstExpressionBoolean;

typedef struct AstArgument AstArgument;

struct AstArgument {
	AstExpression *expression;
	AstArgument *next;
	AstArgument *previous;
};

typedef struct {
	AstExpression *e;
	Token lparen;
	Token rparen;
	AstArgument *arguments;
} AstExpressionCall;

typedef struct {
	AstExpression *e;
	Token operator;
	DataType *to;
} AstExpressionCast;

typedef struct {
	Token value;
} AstExpressionNumber;

typedef struct {
	Token operator;
	AstExpression *e;
} AstExpressionPostfix;

typedef struct {
	Token operator;
	AstExpression *e;
} AstExpressionPrefix;

typedef struct {
	AstExpression *condition;
	AstExpression *a;
	AstExpression *b;
	Token operator;
} AstExpressionTernary;

typedef struct {
	Token identifier;
} AstExpressionVar;

typedef enum {
	AstExpression_Access,
	AstExpression_Assign,
	AstExpression_Binary,
	AstExpression_Boolean,
	AstExpression_Call,
	AstExpression_Cast,
	AstExpression_Number,
	AstExpression_Postfix,
	AstExpression_Prefix,
	AstExpression_Ternary,
	AstExpression_Var,
} AstExpressionType;

struct AstExpression {
	AstExpressionType type;
	union {
		AstExpressionAccess *access;
		AstExpressionAssign *assign;
		AstExpressionBinary *binary;
		AstExpressionBoolean *boolean;
		AstExpressionCall *call;
		AstExpressionCast *cast;
		AstExpressionNumber *number;
		AstExpressionPostfix *postfix;
		AstExpressionPrefix *prefix;
		AstExpressionTernary *ternary;
		AstExpressionVar *var;
	} as;
	DataType *dataType;
	bool modifiable;
};

typedef struct AstStatement AstStatement;

typedef struct {
	AstStatement *children;
	Scope *scope;
} AstStatementBlock;

typedef struct {
	Token keyword;
} AstStatementBreakL;

typedef struct {
	Token keyword;
} AstStatementContinueL;

typedef struct {
	AstStatement *a;
	AstExpression *condition;
	Token keyword;
} AstStatementDoWhile;

typedef struct {
	AstExpression *expression;
} AstStatementExpr;

typedef struct {
	AstExpression *condition;
	AstStatement *a;
	AstStatement *b;
	Token keyword;
} AstStatementIfE;

typedef struct {
	Token identifier;
	DataType *type;
	AstExpression *expression;
	Token operator;
} AstStatementInit;

typedef struct {
	Token keyword;
	AstExpression *expression;
} AstStatementReturnE;

typedef struct {
	Token identifier;
	DataType *type;
} AstStatementVar;

typedef struct {
	AstExpression *condition;
	AstStatement *a;
	Token keyword;
} AstStatementWhileC;

typedef enum {
	AstStatement_Block,
	AstStatement_BreakL,
	AstStatement_ContinueL,
	AstStatement_DoWhile,
	AstStatement_Expr,
	AstStatement_IfE,
	AstStatement_Init,
	AstStatement_ReturnE,
	AstStatement_Var,
	AstStatement_WhileC,
} AstStatementType;

struct AstStatement {
	AstStatementType type;
	union {
		AstStatementBlock *block;
		AstStatementBreakL *breakL;
		AstStatementContinueL *continueL;
		AstStatementDoWhile *doWhile;
		AstStatementExpr *expr;
		AstStatementIfE *ifE;
		AstStatementInit *init;
		AstStatementReturnE *returnE;
		AstStatementVar *var;
		AstStatementWhileC *whileC;
	} as;
	AstStatement* next;
	AstStatement* previous;
};

typedef struct AstDeclaration AstDeclaration;

typedef struct AstParameter AstParameter;

struct AstParameter {
	Token identifier;
	DataType *type;
	AstParameter *next;
	AstParameter *previous;
};

typedef struct {
	Token keyword;
	Token identifier;
	AstStatement *body;
	DataType *dataType;
	AstParameter *parameters;
	Scope *scope;
} AstDeclarationFunction;

typedef struct {
	Token keyword;
	Token identifier;
	AstParameter *members;
	DataType *dataType;
	Scope *scope;
} AstDeclarationStructD;

typedef enum {
	AstDeclaration_Function,
	AstDeclaration_StructD,
} AstDeclarationType;

struct AstDeclaration {
	AstDeclarationType type;
	union {
		AstDeclarationFunction *function;
		AstDeclarationStructD *structD;
	} as;
	AstDeclaration *next;
	AstDeclaration *previous;
};

typedef struct {
	AstDeclaration *declarations;
	Scope *scope;
} AstRoot;

typedef struct {
	AstRoot *root;
} Ast;

Ast *ast_init (AstRoot *root);
AstRoot *ast_initRoot (AstDeclaration *declarations);

AstDeclaration *ast_initDeclarationFunction (Token keyword, Token identifier, AstStatement *body, DataType *returnType, AstParameter *parameters);
AstDeclaration *ast_initDeclarationStructD (Token keyword, Token identifier, AstParameter *members);

AstStatement *ast_initStatementBlock (AstStatement *children);
AstStatement *ast_initStatementBreakL (Token keyword);
AstStatement *ast_initStatementCaseL (AstExpression *e, AstStatement *body, Token keyword);
AstStatement *ast_initStatementContinueL (Token keyword);
AstStatement *ast_initStatementDoWhile (AstStatement *a, AstExpression *condition, Token keyword);
AstStatement *ast_initStatementExpr (AstExpression *expression);
AstStatement *ast_initStatementForI (AstStatement *init, AstExpression* condition, AstExpression *update, AstStatement *body, Token keyword);
AstStatement *ast_initStatementIfE (AstExpression *condition, AstStatement *a, AstStatement *b, Token keyord);
AstStatement *ast_initStatementInit (Token identifier, DataType *type, AstExpression *expression, Token operator);
AstStatement *ast_initStatementReturnE (Token keyword, AstExpression *expression);
AstStatement *ast_initStatementVar (Token identifier, DataType *type);
AstStatement *ast_initStatementWhileC (AstExpression *condition, AstStatement* a, Token keyword);

AstExpression *ast_initExpressionAccess (AstExpression *e, Token op, Token mToken);
AstExpression *ast_initExpressionAssign (AstExpression *a, AstExpression *b, Token operator);
AstExpression *ast_initExpressionBinary (AstExpression *a, AstExpression *b, Token operator);
AstExpression *ast_initExpressionBoolean (bool value);
AstExpression *ast_initExpressionCall (AstExpression *e, Token lparen, Token rparen, AstArgument *arguments);
AstExpression *ast_initExpressionCast (AstExpression *e, Token operator, DataType *to);
AstExpression *ast_initExpressionNumber (Token value);
AstExpression *ast_initExpressionPostfix (Token operator, AstExpression *e);
AstExpression *ast_initExpressionPrefix (Token operator, AstExpression *e);
AstExpression *ast_initExpressionTernary (AstExpression *condition, AstExpression *a, AstExpression *b, Token operator);
AstExpression *ast_initExpressionVar (Token identifier);

AstArgument *ast_initArgument (AstExpression *expression);
AstParameter *ast_initParameter (Token identifier, DataType *type);

#endif

