#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"

struct parser_parser;

struct parser_parser *parser_create (void);
void parser_destroy (struct parser_parser *parser);
void parser_attach (struct parser_parser *parser, struct scanner_scanner *scanner);
void parser_detach (struct parser_parser *parser);
void parser_displayTokens (struct parser_parser *parser);

#endif

