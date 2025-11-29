#ifndef TOKEN_H
#define TOKEN_H

enum token_type {
	token_type_eof,
	token_type_number,
};

struct token_token {
	enum token_type type;
	union {
		struct {
			int value;
		} number;
	} as;
};

char *token_getLabel (enum token_type type);

#endif

