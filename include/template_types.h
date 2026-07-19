#ifndef TEMPLATE_TYPES_H_
#define TEMPLATE_TYPES_H_

#include <stdint.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#define I8_FMT "%" PRIi8
#define I8_FMT_LN I8_FMT "\n"

#define I16_FMT "%" PRIi16
#define I16_FMT_LN I16_FMT "\n"

#define I32_FMT "%" PRIi32
#define I32_FMT_LN I32_FMT "\n"

#define I64_FMT "%" PRIi64
#define I64_FMT_LN I64_FMT "\n"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define U8_FMT "%" PRIu8
#define U8_FMT_LN U8_FMT "\n"
#define U8_FMT_HEX "0x%" PRIx8
#define U8_FMT_HEX_LN U8_FMT_HEX "\n"
#define U8_FMT_OCT "0o%" PRIo8
#define U8_FMT_OCT_LN U8_FMT_OCT "\n"

#define U16_FMT "%" PRIu16
#define U16_FMT_LN U16_FMT "\n"
#define U16_FMT_HEX "0x%" PRIx16
#define U16_FMT_HEX_LN U16_FMT_HEX "\n"
#define U16_FMT_OCT "0o%" PRIo16
#define U16_FMT_OCT_LN U16_FMT_OCT "\n"

#define U32_FMT "%" PRIu32
#define U32_FMT_LN U32_FMT "\n"
#define U32_FMT_HEX "0x%" PRIx32
#define U32_FMT_HEX_LN U32_FMT_HEX "\n"
#define U32_FMT_OCT "0o%" PRIo32
#define U32_FMT_OCT_LN U32_FMT_OCT "\n"

#define U64_FMT "%" PRIu64
#define U64_FMT_LN U64_FMT "\n"
#define U64_FMT_HEX "0x%" PRIx64
#define U64_FMT_HEX_LN U64_FMT_HEX "\n"
#define U64_FMT_OCT "0o%" PRIo64
#define U64_FMT_OCT_LN U64_FMT_OCT "\n"

typedef float f32;
typedef double f64;

#define F32_FMT "%f"
#define F32_FMT_LN F32_FMT "\n"
#define F32_ARG(arg) (f64)(arg)

#define F64_FMT "%f"
#define F64_FMT_LN F64_FMT "\n"

typedef size_t usize;

#define USIZE_FMT "%zu"
#define USIZE_FMT_LN USIZE_FMT "\n"

#define BOOL_FMT "%s"
#define BOOL_FMT_LN BOOL_FMT "\n"
#define BOOL_ARG(arg) ((arg) ? "true" : "false")

#endif
