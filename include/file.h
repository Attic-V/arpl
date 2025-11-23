#ifndef FILE_H
#define FILE_H

struct file_reader;

struct file_reader *file_open (char *path);
int file_getChar (struct file_reader *reader);
void file_close (struct file_reader *reader);

#endif

