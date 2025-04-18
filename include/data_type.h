#ifndef data_type_h
#define data_type_h

#include <stdbool.h>

#include "token.h"

typedef struct DataType DataType;

typedef struct {
} DataTypeBoolean;

typedef struct {
} DataTypeFunction;

typedef struct {
} DataTypeI16;

typedef struct {
} DataTypeI32;

typedef struct {
} DataTypeI64;

typedef struct {
} DataTypeI8;

typedef struct {
	DataType *to;
} DataTypePointer;

typedef struct {
} DataTypeU16;

typedef struct {
} DataTypeU32;

typedef struct {
} DataTypeU64;

typedef struct {
} DataTypeU8;

typedef enum {
	DataType_Boolean,
	DataType_Function,
	DataType_I16,
	DataType_I32,
	DataType_I64,
	DataType_I8,
	DataType_Pointer,
	DataType_U16,
	DataType_U32,
	DataType_U64,
	DataType_U8,

	DataTypeType_Count,
} DataTypeType;

struct DataType {
	DataTypeType type;
	union {
		DataTypeBoolean *boolean;
		DataTypeFunction *function;
		DataTypeI16 *i16;
		DataTypeI32 *i32;
		DataTypeI64 *i64;
		DataTypeI8 *i8;
		DataTypePointer *pointer;
		DataTypeU16 *u16;
		DataTypeU32 *u32;
		DataTypeU64 *u64;
		DataTypeU8 *u8;
	} as;
	bool mutable;
};

DataType *dataType_initBoolean (void);
DataType *dataType_initFunction (void);
DataType *dataType_initI16 (void);
DataType *dataType_initI32 (void);
DataType *dataType_initI64 (void);
DataType *dataType_initI8 (void);
DataType *dataType_initPointer (DataType *to);
DataType *dataType_initU16 (void);
DataType *dataType_initU32 (void);
DataType *dataType_initU64 (void);
DataType *dataType_initU8 (void);

DataType *dataType_smallestInt (size_t value);

bool dataType_isBoolean (DataType *t);
bool dataType_isFunction (DataType *t);
bool dataType_isI16 (DataType *t);
bool dataType_isI32 (DataType *t);
bool dataType_isI64 (DataType *t);
bool dataType_isI8 (DataType *t);
bool dataType_isPointer (DataType *t);
bool dataType_isU16 (DataType *t);
bool dataType_isU32 (DataType *t);
bool dataType_isU64 (DataType *t);
bool dataType_isU8 (DataType *t);

bool dataType_isInt (DataType *t);
bool dataType_isIntSigned (DataType *t);
bool dataType_isIntUnsigned (DataType *t);

bool dataType_castable (DataType *from, DataType *to);
bool dataType_equal (DataType *a, DataType *b);
size_t dataType_getSize (DataType *type);
DataType *dataType_larger (DataType *a, DataType *b);

#endif
