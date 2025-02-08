#ifndef token_h
#define token_h

#include <stddef.h>

typedef enum {
	TT_EOF,
	TT_Equal_Equal,
	TT_False,
	TT_Identifier,
	TT_LParen,
	TT_Minus,
	TT_Number,
	TT_Plus,
	TT_RParen,
	TT_Star,
	TT_True,
} TokenType;

typedef struct {
	int line;
	char *lexeme;
	int length;
	TokenType type;
} Token;

#endif
