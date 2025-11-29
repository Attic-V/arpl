#ifndef SCANNER_INTERNAL_H
#define SCANNER_INTERNAL_H

#include "scanner.h"

#include <stdio.h>
#include <stdlib.h>

struct scanner_scanner {
	struct file_reader *reader;
};

enum token_type peekType (struct scanner_scanner *scanner);

#endif

