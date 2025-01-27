#ifndef asm_generator_h
#define asm_generator_h

#include "arena.h"
#include "ir.h"

void gen_x86 (Arena *arena, Ir *ir);

#endif
