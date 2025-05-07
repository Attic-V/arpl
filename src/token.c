#include <string.h>

#include "token.h"

bool token_equal (Token a, Token b)
{
	if (a.length != b.length) {
		return false;
	}
	if (strncmp(a.lexeme, b.lexeme, a.length)) {
		return false;
	}
	return true;
}
