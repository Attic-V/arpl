#ifndef SCANNER_INTERNAL_H
#define SCANNER_INTERNAL_H

#include "scanner.h"

#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

struct scanner_scanner {
	struct file_reader *reader;
	struct queue_queue *delayed;
};

#endif

