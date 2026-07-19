#ifndef TEMPLATE_TYPES_H_
#define TEMPLATE_TYPES_H_

/**
 * @file template_types.h
 * @brief Short fixed-width type aliases and matching printf-style format
 *        macros.
 */

#include <stdint.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * @typedef i8
 * @brief 8-bit signed integer. Alias for `int8_t`.
 */
typedef int8_t i8;

/**
 * @typedef i16
 * @brief 16-bit signed integer. Alias for `int16_t`.
 */
typedef int16_t i16;

/**
 * @typedef i32
 * @brief 32-bit signed integer. Alias for `int32_t`.
 */
typedef int32_t i32;

/**
 * @typedef i64
 * @brief 64-bit signed integer. Alias for `int64_t`.
 */
typedef int64_t i64;

/**
 * @def I8_FMT
 * @brief printf conversion specifier for @ref i8.
 */
#define I8_FMT "%" PRIi8

/**
 * @def I8_FMT_LN
 * @brief @ref I8_FMT with a trailing newline.
 */
#define I8_FMT_LN I8_FMT "\n"

/**
 * @def I16_FMT
 * @brief printf conversion specifier for @ref i16.
 */
#define I16_FMT "%" PRIi16

/**
 * @def I16_FMT_LN
 * @brief @ref I16_FMT with a trailing newline.
 */
#define I16_FMT_LN I16_FMT "\n"

/**
 * @def I32_FMT
 * @brief printf conversion specifier for @ref i32.
 */
#define I32_FMT "%" PRIi32

/**
 * @def I32_FMT_LN
 * @brief @ref I32_FMT with a trailing newline.
 */
#define I32_FMT_LN I32_FMT "\n"

/**
 * @def I64_FMT
 * @brief printf conversion specifier for @ref i64.
 */
#define I64_FMT "%" PRIi64

/**
 * @def I64_FMT_LN
 * @brief @ref I64_FMT with a trailing newline.
 */
#define I64_FMT_LN I64_FMT "\n"

/**
 * @typedef u8
 * @brief 8-bit unsigned integer. Alias for `uint8_t`.
 */
typedef uint8_t u8;

/**
 * @typedef u16
 * @brief 16-bit unsigned integer. Alias for `uint16_t`.
 */
typedef uint16_t u16;

/**
 * @typedef u32
 * @brief 32-bit unsigned integer. Alias for `uint32_t`.
 */
typedef uint32_t u32;

/**
 * @typedef u64
 * @brief 64-bit unsigned integer. Alias for `uint64_t`.
 */
typedef uint64_t u64;

/**
 * @def U8_FMT
 * @brief Decimal printf conversion specifier for @ref u8.
 */
#define U8_FMT "%" PRIu8

/**
 * @def U8_FMT_LN
 * @brief @ref U8_FMT with a trailing newline.
 */
#define U8_FMT_LN U8_FMT "\n"

/**
 * @def U8_FMT_HEX
 * @brief Hexadecimal printf conversion specifier for @ref u8, prefixed
 *        with `0x`.
 */
#define U8_FMT_HEX "0x%" PRIx8

/**
 * @def U8_FMT_HEX_LN
 * @brief @ref U8_FMT_HEX with a trailing newline.
 */
#define U8_FMT_HEX_LN U8_FMT_HEX "\n"

/**
 * @def U8_FMT_OCT
 * @brief Octal printf conversion specifier for @ref u8, prefixed with a
 *        literal `0o`.
 *
 * @note The `0o` prefix is a literal two-character string, not a C octal
 *       literal prefix (C uses a bare leading `0`, not `0o`). This is
 *       purely a display convention chosen for readability/disambiguation
 *       from decimal, not something `strtol`/`scanf` would parse back
 *       with base auto-detection.
 */
#define U8_FMT_OCT "0o%" PRIo8

/**
 * @def U8_FMT_OCT_LN
 * @brief @ref U8_FMT_OCT with a trailing newline.
 */
#define U8_FMT_OCT_LN U8_FMT_OCT "\n"

/**
 * @def U16_FMT
 * @brief Decimal printf conversion specifier for @ref u16.
 */
#define U16_FMT "%" PRIu16

/**
 * @def U16_FMT_LN
 * @brief @ref U16_FMT with a trailing newline.
 */
#define U16_FMT_LN U16_FMT "\n"

/**
 * @def U16_FMT_HEX
 * @brief Hexadecimal printf conversion specifier for @ref u16, prefixed
 *        with `0x`.
 */
#define U16_FMT_HEX "0x%" PRIx16

/**
 * @def U16_FMT_HEX_LN
 * @brief @ref U16_FMT_HEX with a trailing newline.
 */
#define U16_FMT_HEX_LN U16_FMT_HEX "\n"

/**
 * @def U16_FMT_OCT
 * @brief Octal printf conversion specifier for @ref u16, prefixed with a
 *        literal `0o` (see @ref U8_FMT_OCT for the prefix caveat).
 */
#define U16_FMT_OCT "0o%" PRIo16

/**
 * @def U16_FMT_OCT_LN
 * @brief @ref U16_FMT_OCT with a trailing newline.
 */
#define U16_FMT_OCT_LN U16_FMT_OCT "\n"

/**
 * @def U32_FMT
 * @brief Decimal printf conversion specifier for @ref u32.
 */
#define U32_FMT "%" PRIu32

/**
 * @def U32_FMT_LN
 * @brief @ref U32_FMT with a trailing newline.
 */
#define U32_FMT_LN U32_FMT "\n"

/**
 * @def U32_FMT_HEX
 * @brief Hexadecimal printf conversion specifier for @ref u32, prefixed
 *        with `0x`.
 */
#define U32_FMT_HEX "0x%" PRIx32

/**
 * @def U32_FMT_HEX_LN
 * @brief @ref U32_FMT_HEX with a trailing newline.
 */
#define U32_FMT_HEX_LN U32_FMT_HEX "\n"

/**
 * @def U32_FMT_OCT
 * @brief Octal printf conversion specifier for @ref u32, prefixed with a
 *        literal `0o` (see @ref U8_FMT_OCT for the prefix caveat).
 */
#define U32_FMT_OCT "0o%" PRIo32

/**
 * @def U32_FMT_OCT_LN
 * @brief @ref U32_FMT_OCT with a trailing newline.
 */
#define U32_FMT_OCT_LN U32_FMT_OCT "\n"

/**
 * @def U64_FMT
 * @brief Decimal printf conversion specifier for @ref u64.
 */
#define U64_FMT "%" PRIu64

/**
 * @def U64_FMT_LN
 * @brief @ref U64_FMT with a trailing newline.
 */
#define U64_FMT_LN U64_FMT "\n"

/**
 * @def U64_FMT_HEX
 * @brief Hexadecimal printf conversion specifier for @ref u64, prefixed
 *        with `0x`.
 */
#define U64_FMT_HEX "0x%" PRIx64

/**
 * @def U64_FMT_HEX_LN
 * @brief @ref U64_FMT_HEX with a trailing newline.
 */
#define U64_FMT_HEX_LN U64_FMT_HEX "\n"

/**
 * @def U64_FMT_OCT
 * @brief Octal printf conversion specifier for @ref u64, prefixed with a
 *        literal `0o` (see @ref U8_FMT_OCT for the prefix caveat).
 */
#define U64_FMT_OCT "0o%" PRIo64

/**
 * @def U64_FMT_OCT_LN
 * @brief @ref U64_FMT_OCT with a trailing newline.
 */
#define U64_FMT_OCT_LN U64_FMT_OCT "\n"

/**
 * @typedef f32
 * @brief Single-precision floating point. Alias for `float`.
 */
typedef float f32;

/**
 * @typedef f64
 * @brief Double-precision floating point. Alias for `double`.
 */
typedef double f64;

/**
 * @def F32_FMT
 * @brief printf conversion specifier for a floating-point value.
 *
 * @warning `printf`'s variadic argument-promotion rules mean a bare
 *          `f32`/`float` argument is promoted to `double` when passed
 *          through `...`, so `%g` (not a distinct `float`-specific
 *          specifier) is correct here — but the argument itself must
 *          actually be promoted to `double`. See @ref F32_ARG.
 */
#define F32_FMT "%g"

/**
 * @def F32_FMT_LN
 * @brief @ref F32_FMT with a trailing newline.
 */
#define F32_FMT_LN F32_FMT "\n"

/**
 * @def F32_ARG(arg)
 * @brief Explicitly promotes an @ref f32 argument to `f64` before
 *        passing it to a variadic function.
 *
 * @param arg An `f32`/`float`-typed expression.
 * @return `arg`, cast to `f64`.
 *
 * @note Default C variadic argument promotion already promotes `float`
 *       to `double` automatically, so this cast is not strictly required
 *       for correctness in a plain `printf(F32_FMT, x)` call. It exists
 *       to make the promotion explicit at the call site (documenting
 *       intent, and silencing `-Wdouble-promotion`, which this project
 *       builds with and which would otherwise warn on the implicit
 *       promotion).
 */
#define F32_ARG(arg) (f64)(arg)

/**
 * @def F64_FMT
 * @brief printf conversion specifier for an @ref f64 value.
 */
#define F64_FMT "%g"

/**
 * @def F64_FMT_LN
 * @brief @ref F64_FMT with a trailing newline.
 */
#define F64_FMT_LN F64_FMT "\n"

/**
 * @typedef usize
 * @brief Unsigned pointer-sized integer, suitable for sizes/indices/counts.
 *        Alias for `size_t`.
 */
typedef size_t usize;

/**
 * @def USIZE_FMT
 * @brief printf conversion specifier for @ref usize.
 *
 * @note Uses the standard `%zu` specifier directly rather than an
 *       `<inttypes.h>` macro, since `size_t`'s width is already handled
 *       portably by the `z` length modifier in C99 and later.
 */
#define USIZE_FMT "%zu"

/**
 * @def USIZE_FMT_LN
 * @brief @ref USIZE_FMT with a trailing newline.
 */
#define USIZE_FMT_LN USIZE_FMT "\n"

/**
 * @def BOOL_FMT
 * @brief printf conversion specifier for a `bool` value formatted via
 *        @ref BOOL_ARG.
 *
 * @note `bool` has no native printf conversion specifier — C promotes it
 *       to `int` in a variadic call, which would print `0`/`1` rather
 *       than `false`/`true`. This macro expects its corresponding
 *       argument to already be a `const char *` (produced by
 *       @ref BOOL_ARG), hence the `%s` specifier.
 */
#define BOOL_FMT "%s"

/**
 * @def BOOL_FMT_LN
 * @brief @ref BOOL_FMT with a trailing newline.
 */
#define BOOL_FMT_LN BOOL_FMT "\n"

/**
 * @def BOOL_ARG(arg)
 * @brief Converts a `bool` expression into a `"true"`/`"false"` string
 *        literal, for use with @ref BOOL_FMT.
 *
 * @param arg A `bool`-valued (or boolean-context) expression.
 * @return `"true"` if `arg` is truthy, `"false"` otherwise.
 *
 * Example:
 * ```
 * bool ok = true;
 * printf(BOOL_FMT_LN, BOOL_ARG(ok)); // prints "true\n"
 * ```
 */
#define BOOL_ARG(arg) ((arg) ? "true" : "false")

#endif