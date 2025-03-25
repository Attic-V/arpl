#ifndef asm_generator_h
#define asm_generator_h

#include "ir.h"

extern void (*CastTable[DataTypeType_Count][DataTypeType_Count])(DataType *from, DataType *to);

void gen_x86 (Ir *ir);

#endif
