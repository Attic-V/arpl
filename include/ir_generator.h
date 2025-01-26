#ifndef ir_generator_h
#define ir_generator_h

#include "arena.h"
#include "ast.h"
#include "tac.h"

Tac *tac_generate (Arena *arena, Ast *ast);

#endif
