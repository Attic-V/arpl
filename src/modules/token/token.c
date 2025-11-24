#include "internal.h"

char *token_getLabel (enum token_type type)
{
	switch (type) {
		case token_eof:
			return "EOF";
		default:
			return "unknown";
	}
}

