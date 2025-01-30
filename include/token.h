#ifndef token_h
#define token_h

#include <stddef.h>

typedef enum {
	TT_EOF,
	TT_Number,
	TT_Plus,
} TokenType;

typedef struct {
	int line;
	char *lexeme;
	int length;
	TokenType type;
} Token;

#endif
