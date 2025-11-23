#include "internal.h"

struct file_reader *file_open (char *path)
{
	FILE *file = fopen(path, "rb");
	if (!file) {
		fprintf(stderr, "could not open file '%s': %s\n", path, strerror(errno));
		return NULL;
	}

	struct file_reader *reader = createReader();
	if (!reader) {
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

	destroyReader(reader);
}

struct file_reader *createReader (void)
{
	struct file_reader *reader = malloc(sizeof(struct file_reader));
	if (!reader) {
		perror("memory allocation failed");
		return NULL;
	}

	reader->file = NULL;

	return reader;
}

void destroyReader (struct file_reader *reader)
{
	if (!reader) return;

	free(reader);
}

