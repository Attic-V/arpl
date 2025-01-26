#ifndef tac_h
#define tac_h

#include "arena.h"
#include "token.h"

typedef struct Tac Tac;
typedef struct TacInstruction TacInstruction;

Tac *tac_init (Arena *arena);
TacInstruction *tac_init_const (Arena *arena, Token value);

void tac_set_head (Tac *tac, TacInstruction *head);

#endif
