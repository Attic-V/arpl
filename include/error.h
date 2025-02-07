#ifndef error_h
#define error_h

#include <stdarg.h>

#include "token.h"

void error (Token location, char *format, ...);
void verror (Token location, char *format, va_list args);

#endif
