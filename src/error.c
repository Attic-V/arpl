#include <stdarg.h>
#include <stdio.h>

#include "error.h"
#include "token.h"

void err (Token location, char *format, ...)
{
	va_list args;
	va_start(args, format);
	verr(location, format, args);
	va_end(args);
}

void verr (Token location, char *format, va_list args)
{
	if (location.type != TT_EOF) {
		fprintf(stderr, "%d: ", location.line);
	}
	fprintf(stderr, "error at ");
	if (location.type == TT_EOF) {
		fprintf(stderr, "EOF");
	} else {
		fprintf(stderr, "'%.*s'", location.length, location.lexeme);
	}
	fprintf(stderr, ": ");
	vfprintf(stderr, format, args);
	fprintf(stderr, "\n");
}
