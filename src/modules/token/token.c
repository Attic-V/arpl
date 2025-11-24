#include "internal.h"

char *token_getLabel (enum token_tokentype tokentype)
{
	switch (tokentype) {
		case token_eof:
			return "EOF";
		default:
			return "unknown";
	}
}

