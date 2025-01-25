#ifndef ast_h
#define ast_h

#include "arena.h"
#include "token.h"

typedef struct Ast Ast;

Ast *ast_init (Arena *arena);
void ast_setValue (Ast *ast, Token value);

#endif
