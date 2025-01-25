#ifndef ast_h
#define ast_h

#include "arena.h"
#include "token.h"

typedef struct Ast Ast;

Ast *ast_initLiteral (Arena *arena, Token value);
Ast *ast_initRoot (Arena *arena, Ast *literal);

#endif
