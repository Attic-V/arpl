#ifndef ast_h
#define ast_h

#include "arena.h"
#include "token.h"

typedef struct AstLiteral AstLiteral;
typedef struct AstRoot AstRoot;

AstLiteral *ast_initLiteral (Arena *arena, Token value);
AstRoot *ast_initRoot (Arena *arena, AstLiteral *literal);

#endif
