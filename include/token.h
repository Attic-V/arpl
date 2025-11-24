#ifndef TOKEN_H
#define TOKEN_H

enum token_tokentype {
	token_eof,
};

struct token_token {
	enum token_tokentype type;
	char ch;
};

#endif

