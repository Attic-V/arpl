#include "data.h"
#include "data_type.h"
#include "memory.h"
#include "x86_gen.h"

static DataType *dataType_init (DataTypeType type);

static DataTypeBoolean *dataTypeBoolean_init (void);
static DataTypeFunction *dataTypeFunction_init (DataType *returnType);
static DataTypeI16 *dataTypeI16_init (void);
static DataTypeI32 *dataTypeI32_init (void);
static DataTypeI64 *dataTypeI64_init (void);
static DataTypeI8 *dataTypeI8_init (void);
static DataTypePointer *dataTypePointer_init (DataType *to);
static DataTypeU16 *dataTypeU16_init (void);
static DataTypeU32 *dataTypeU32_init (void);
static DataTypeU64 *dataTypeU64_init (void);
static DataTypeU8 *dataTypeU8_init (void);

static DataType *dataType_init (DataTypeType type)
{
	DataType *t = mem_alloc(sizeof(*t));
	t->type = type;
	t->mutable = true;
	return t;
}

DataType *dataType_initBoolean (void)
{
	DataType *t = dataType_init(DataType_Boolean);
	t->as.boolean = dataTypeBoolean_init();
	return t;
}

static DataTypeBoolean *dataTypeBoolean_init (void)
{
	DataTypeBoolean *t = mem_alloc(sizeof(*t));
	return t;
}

DataType *dataType_initFunction (DataType *returnType)
{
	DataType *t = dataType_init(DataType_Function);
	t->as.function = dataTypeFunction_init(returnType);
	return t;
}

static DataTypeFunction *dataTypeFunction_init (DataType *returnType)
{
	DataTypeFunction *t = mem_alloc(sizeof(*t));
	t->returnType = returnType;
	return t;
}

DataType *dataType_initI16 (void)
{
	DataType *t = dataType_init(DataType_I16);
	t->as.i16 = dataTypeI16_init();
	return t;
}

static DataTypeI16 *dataTypeI16_init (void)
{
	DataTypeI16 *t = mem_alloc(sizeof(*t));
	return t;
}

DataType *dataType_initI32 (void)
{
	DataType *t = dataType_init(DataType_I32);
	t->as.i32 = dataTypeI32_init();
	return t;
}

static DataTypeI32 *dataTypeI32_init (void)
{
	DataTypeI32 *t = mem_alloc(sizeof(*t));
	return t;
}

DataType *dataType_initI64 (void)
{
	DataType *t = dataType_init(DataType_I64);
	t->as.i64 = dataTypeI64_init();
	return t;
}

static DataTypeI64 *dataTypeI64_init (void)
{
	DataTypeI64 *t = mem_alloc(sizeof(*t));
	return t;
}

DataType *dataType_initI8 (void)
{
	DataType *t = dataType_init(DataType_I8);
	t->as.i8 = dataTypeI8_init();
	return t;
}

static DataTypeI8 *dataTypeI8_init (void)
{
	DataTypeI8 *t = mem_alloc(sizeof(*t));
	return t;
}

DataType *dataType_initPointer (DataType *to)
{
	DataType *t = dataType_init(DataType_Pointer);
	t->as.pointer = dataTypePointer_init(to);
	return t;
}

static DataTypePointer *dataTypePointer_init (DataType *to)
{
	DataTypePointer *t = mem_alloc(sizeof(*t));
	t->to = to;
	return t;
}

DataType *dataType_initU16 (void)
{
	DataType *t = dataType_init(DataType_U16);
	t->as.u16 = dataTypeU16_init();
	return t;
}

static DataTypeU16 *dataTypeU16_init (void)
{
	DataTypeU16 *t = mem_alloc(sizeof(*t));
	return t;
}

DataType *dataType_initU32 (void)
{
	DataType *t = dataType_init(DataType_U32);
	t->as.u32 = dataTypeU32_init();
	return t;
}

static DataTypeU32 *dataTypeU32_init (void)
{
	DataTypeU32 *t = mem_alloc(sizeof(*t));
	return t;
}

DataType *dataType_initU64 (void)
{
	DataType *t = dataType_init(DataType_U64);
	t->as.u64 = dataTypeU64_init();
	return t;
}

static DataTypeU64 *dataTypeU64_init (void)
{
	DataTypeU64 *t = mem_alloc(sizeof(*t));
	return t;
}

DataType *dataType_initU8 (void)
{
	DataType *t = dataType_init(DataType_U8);
	t->as.u8 = dataTypeU8_init();
	return t;
}

static DataTypeU8 *dataTypeU8_init (void)
{
	DataTypeU8 *t = mem_alloc(sizeof(*t));
	return t;
}

bool dataType_isBoolean (DataType *t)
{
	return t->type == DataType_Boolean;
}

bool dataType_isFunction (DataType *t)
{
	return t->type == DataType_Function;
}

bool dataType_isI16 (DataType *t)
{
	return t->type == DataType_I16;
}

bool dataType_isI32 (DataType *t)
{
	return t->type == DataType_I32;
}

bool dataType_isI64 (DataType *t)
{
	return t->type == DataType_I64;
}

bool dataType_isI8 (DataType *t)
{
	return t->type == DataType_I8;
}

bool dataType_isPointer (DataType *t)
{
	return t->type == DataType_Pointer;
}

bool dataType_isU16 (DataType *t)
{
	return t->type == DataType_U16;
}

bool dataType_isU32 (DataType *t)
{
	return t->type == DataType_U32;
}

bool dataType_isU64 (DataType *t)
{
	return t->type == DataType_U64;
}

bool dataType_isU8 (DataType *t)
{
	return t->type == DataType_U8;
}

bool dataType_isInt (DataType *t)
{
	return dataType_isIntSigned(t) || dataType_isIntUnsigned(t);
}

bool dataType_isIntSigned (DataType *t)
{
	return dataType_isI8(t) || dataType_isI16(t) || dataType_isI32(t) || dataType_isI64(t);
}

bool dataType_isIntUnsigned (DataType *t)
{
	return dataType_isU8(t) || dataType_isU16(t) || dataType_isU32(t) || dataType_isU64(t);
}

bool dataType_castable (DataType *from, DataType *to)
{
	return CastTable[from->type][to->type] != NULL;
}

bool dataType_equal (DataType *a, DataType *b)
{
	if (a->mutable != b->mutable) {
		return false;
	}
	if (a->type != b->type) {
		return false;
	}
	switch (a->type) {
		case DataType_Boolean: return true;
		case DataType_I16: return true;
		case DataType_I32: return true;
		case DataType_I64: return true;
		case DataType_I8: return true;
		case DataType_Pointer: return dataType_equal(a->as.pointer->to, b->as.pointer->to);
		case DataType_U16: return true;
		case DataType_U32: return true;
		case DataType_U64: return true;
		case DataType_U8: return true;
		default:
	}
	return false;
}

size_t dataType_getSize (DataType *t)
{
	switch (t->type) {
		case DataType_Boolean: return BYTE;
		case DataType_I16: return WORD;
		case DataType_I32: return DWORD;
		case DataType_I64: return QWORD;
		case DataType_I8: return BYTE;
		case DataType_Pointer: return QWORD;
		case DataType_U16: return WORD;
		case DataType_U32: return DWORD;
		case DataType_U64: return QWORD;
		case DataType_U8: return BYTE;
		default:
	}
	return -1;
}

DataType *dataType_smallestInt (size_t value)
{
	size_t numBits = 0;
	for (size_t n = value; n != 0; n >>= 1, numBits++);
	if (numBits <= 8 * BYTE) return dataType_initI8();
	if (numBits <= 8 * WORD) return dataType_initI16();
	if (numBits <= 8 * DWORD) return dataType_initI32();
	if (numBits <= 8 * QWORD) return dataType_initI64();
	return NULL;
}

DataType *dataType_larger (DataType *a, DataType *b)
{
	return dataType_getSize(a) > dataType_getSize(b) ? a : b;
}
