#include "internal.h"

struct file_reader *file_open (char *path)
{
	FILE *file = fopen(path, "rb");
	if (!file) {
		fprintf(stderr, "could not open file '%s': %s\n", path, strerror(errno));
		return NULL;
	}

	struct file_reader *reader = malloc(sizeof(struct file_reader));
	if (!reader) {
		perror("memory allocation failed");
		fclose(file);
		return NULL;
	}
	reader->file = file;

	return reader;
}

int file_getChar (struct file_reader *reader)
{
	if (!reader || !reader->file) return '\0';

	return fgetc(reader->file);
}

void file_close (struct file_reader *reader)
{
	if (!reader) return;

	if (reader->file) {
		fclose(reader->file);
	}

	free(reader);
}

int file_peekChar (struct file_reader *reader)
{
	int c = file_getChar(reader);
	if (c == EOF) {
		return EOF;
	}
	return ungetc(c, reader->file);
}

