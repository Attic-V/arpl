#ifndef tac_generator_h
#define tac_generator_h

#include "arena.h"
#include "ast.h"
#include "tac.h"

Tac *tac_generate (Arena *arena, Ast *ast);

#endif
