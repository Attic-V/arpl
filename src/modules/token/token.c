#include "internal.h"

char *token_getLabel (enum token_type type)
{
	switch (type) {
		case token_type_eof: return "eof";
		case token_type_newline: return "newline";
		case token_type_number: return "number";
		case token_type_unexpected: return "unexpected";
	}
	return "unknown";
}

