#ifndef data_type_h
#define data_type_h

#include "token.h"

typedef enum {
	DT_Boolean,
	DT_Number,
} DataType;

DataType getTypeFromKey (Token keyword);

#endif
