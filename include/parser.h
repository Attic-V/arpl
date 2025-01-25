#ifndef parser_h
#define parser_h

#include "arena.h"
#include "ast.h"
#include "token.h"

Ast *parse (Arena *arena, Token *tokens);

#endif
