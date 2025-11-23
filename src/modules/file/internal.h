#ifndef FILE_INTERNAL_H
#define FILE_INTERNAL_H

#include "file.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct file_reader {
	FILE *file;
};

struct file_reader *createReader (void);
void destroyReader (struct file_reader *reader);

#endif

