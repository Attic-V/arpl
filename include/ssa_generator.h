#ifndef ssa_generator_h
#define ssa_generator_h

#include "arena.h"
#include "ssa.h"
#include "tac.h"

Ssa *ssa_generate (Arena *arena, Tac *tac);

#endif
