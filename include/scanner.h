#ifndef SCANNER_H
#define SCANNER_H

#include "file.h"

#include "token.h"

struct scanner_scanner;

struct scanner_scanner *scanner_create (void);
void scanner_destroy (struct scanner_scanner *scanner);
void scanner_attach (struct scanner_scanner *scanner, struct file_reader *reader);
void scanner_detach (struct scanner_scanner *scanner);
enum token_tokentype scanner_getTokentype (struct scanner_scanner *scanner);

#endif

