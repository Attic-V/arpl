#ifndef TOKEN_H
#define TOKEN_H

enum token_tokentype {
	token_eof,
};

char *token_getLabel (enum token_tokentype tokentype);

#endif

