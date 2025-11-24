#ifndef SCANNER_INTERNAL_H
#define SCANNER_INTERNAL_H

#include "scanner.h"

#include <stdio.h>
#include <stdlib.h>

struct scanner_scanner {
	struct file_reader *reader;
	int row;
};

#endif

