#ifndef TOKEN_H
#define TOKEN_H

enum token_type {
	token_type_eof,
	token_type_number,
	token_type_unexpected,
};

struct token_token {
	enum token_type type;
	union {
		struct {
			int value;
		} number;
		struct {
			char c;
		} unexpected;
	} as;
};

char *token_getLabel (enum token_type type);

#endif

