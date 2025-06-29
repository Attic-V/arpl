#ifndef err_h
#define err_h

#include <stdarg.h>

#include "token.h"

void err (Token location, char *format, ...);
void verr (Token location, char *format, va_list args);

#endif
