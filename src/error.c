#include <stdio.h>

#include "error.h"

void error (Token location, char *format, ...)
{
	va_list args;
	va_start(args, format);
	verror(location, format, args);
	va_end(args);
}

void verror (Token location, char *format, va_list args)
{
	fprintf(stderr, "%d: error at ", location.line);
	if (location.type == TT_EOF) {
		fprintf(stderr, "EOF");
	} else {
		fprintf(stderr, "'%.*s'", location.length, location.lexeme);
	}
	fprintf(stderr, ": ");
	vfprintf(stderr, format, args);
	fprintf(stderr, "\n");
}
