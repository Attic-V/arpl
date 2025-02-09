#ifndef token_h
#define token_h

#include <stddef.h>

typedef enum {
	TT_And,
	TT_And_And,
	TT_Bang,
	TT_Bang_Equal,
	TT_Caret,
	TT_Colon,
	TT_EOF,
	TT_Equal_Equal,
	TT_False,
	TT_Greater_Greater,
	TT_Identifier,
	TT_Less,
	TT_Less_Equal,
	TT_Less_Less,
	TT_LParen,
	TT_Minus,
	TT_Number,
	TT_Pipe,
	TT_Pipe_Pipe,
	TT_Plus,
	TT_Question,
	TT_RParen,
	TT_Star,
	TT_Tilde,
	TT_True,
} TokenType;

typedef struct {
	int line;
	char *lexeme;
	int length;
	TokenType type;
} Token;

#endif
