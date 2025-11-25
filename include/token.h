#ifndef TOKEN_H
#define TOKEN_H

enum token_type {
	token_type_eof,
};

struct token_token {
	enum token_type type;
};

char *token_getLabel (enum token_type type);

#endif

