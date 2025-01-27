#ifndef ir_generator_h
#define ir_generator_h

#include "arena.h"
#include "ast.h"
#include "ir.h"

Ir *ir_generate (Arena *arena, Ast *ast);

#endif
