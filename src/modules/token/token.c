#include "internal.h"

char *token_getLabel (enum token_type type)
{
	switch (type) {
		case token_type_eof: return "eof";
		default: return "unknown";
	}
}

