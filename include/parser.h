#ifndef parser_h
#define parser_h

#include "arena.h"
#include "ast.h"
#include "token.h"

AstRoot *parse (Arena *arena, Token *tokens);

#endif
