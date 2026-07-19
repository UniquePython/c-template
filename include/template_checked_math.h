#ifndef TEMPLATE_CHECKED_MATH_H_
#define TEMPLATE_CHECKED_MATH_H_

#include <math.h>

/**
 * @file template_checked_math.h
 * @brief Overflow-safe arithmetic helpers for every fixed-width integer
 *        type in @ref template_types.h, including floating-point.
 *
 * Each function performs the named operation on a specific type and
 * reports whether the result is representable, rather than silently
 * wrapping (unsigned overflow) or invoking undefined behavior (signed
 * overflow).
 *
 * Naming convention: `TemplateChecked<Op><Type>`, e.g.
 * `TemplateCheckedAddI32`, `TemplateCheckedMulUsize`. There is no
 * `_Generic`-based dispatch macro — callers name the exact typed
 * function they want, so a call site is always unambiguous about which
 * type is involved and mismatched-type arguments are caught by ordinary
 * C type checking rather than silently resolved by dispatch.
 *
 * Add/sub/mul are implemented via GCC/Clang's `__builtin_*_overflow`
 * intrinsics. Division and modulo have no such intrinsic (unsigned
 * division/modulo cannot overflow — the only failure mode is a zero
 * divisor) and are implemented as a plain zero-check followed by the
 * operation.
 *
 * Floating-point types have no equivalent overflow intrinsic —
 * `__builtin_*_overflow` only accepts integral types. Instead, the
 * float/double `Checked*` functions perform the operation and then
 * inspect the *result*: an `inf`/`nan` that appears in the result but
 * was not already present in either input is treated as this
 * operation's failure. An `inf`/`nan` that was already present in an
 * input (and simply propagates through unchanged) is not treated as a
 * new failure, since the caller presumably already knows about it.
 *
 * All functions are `static inline` (header-only, no matching `.c`
 * file) — each body is a short wrapper around a compiler intrinsic, a
 * zero-check, or (for floating-point) a post-hoc `isinf`/`isnan`
 * classification of the result, so there is nothing to gain from a
 * separate translation unit, and inlining avoids a real call-boundary in
 * what may be a hot-ish path (e.g. allocation size computation).
 *
 * For every function below: on failure, `*out` is left unmodified (not
 * zeroed) — callers must check the return value before reading `*out`.
 */

#include "template_types.h"

/* ---------------------------------------------------------------- i8 */

/** @brief Computes `a + b` for @ref i8, checked for overflow. */
static inline bool TemplateCheckedAddI8(i8 a, i8 b, i8 *out)
{
    return !__builtin_add_overflow(a, b, out);
}

/** @brief Computes `a - b` for @ref i8, checked for overflow. */
static inline bool TemplateCheckedSubI8(i8 a, i8 b, i8 *out)
{
    return !__builtin_sub_overflow(a, b, out);
}

/** @brief Computes `a * b` for @ref i8, checked for overflow. */
static inline bool TemplateCheckedMulI8(i8 a, i8 b, i8 *out)
{
    return !__builtin_mul_overflow(a, b, out);
}

/** @brief Computes `a / b` for @ref i8, checked for division by zero. */
static inline bool TemplateCheckedDivI8(i8 a, i8 b, i8 *out)
{
    if (b == 0)
        return false;

    *out = (i8)(a / b);
    return true;
}

/** @brief Computes `a %% b` for @ref i8, checked for modulo by zero. */
static inline bool TemplateCheckedModI8(i8 a, i8 b, i8 *out)
{
    if (b == 0)
        return false;

    *out = (i8)(a % b);
    return true;
}

/* --------------------------------------------------------------- i16 */

/** @brief Computes `a + b` for @ref i16, checked for overflow. */
static inline bool TemplateCheckedAddI16(i16 a, i16 b, i16 *out)
{
    return !__builtin_add_overflow(a, b, out);
}

/** @brief Computes `a - b` for @ref i16, checked for overflow. */
static inline bool TemplateCheckedSubI16(i16 a, i16 b, i16 *out)
{
    return !__builtin_sub_overflow(a, b, out);
}

/** @brief Computes `a * b` for @ref i16, checked for overflow. */
static inline bool TemplateCheckedMulI16(i16 a, i16 b, i16 *out)
{
    return !__builtin_mul_overflow(a, b, out);
}

/** @brief Computes `a / b` for @ref i16, checked for division by zero. */
static inline bool TemplateCheckedDivI16(i16 a, i16 b, i16 *out)
{
    if (b == 0)
        return false;

    *out = (i16)(a / b);
    return true;
}

/** @brief Computes `a %% b` for @ref i16, checked for modulo by zero. */
static inline bool TemplateCheckedModI16(i16 a, i16 b, i16 *out)
{
    if (b == 0)
        return false;

    *out = (i16)(a % b);
    return true;
}

/* --------------------------------------------------------------- i32 */

/** @brief Computes `a + b` for @ref i32, checked for overflow. */
static inline bool TemplateCheckedAddI32(i32 a, i32 b, i32 *out)
{
    return !__builtin_add_overflow(a, b, out);
}

/** @brief Computes `a - b` for @ref i32, checked for overflow. */
static inline bool TemplateCheckedSubI32(i32 a, i32 b, i32 *out)
{
    return !__builtin_sub_overflow(a, b, out);
}

/** @brief Computes `a * b` for @ref i32, checked for overflow. */
static inline bool TemplateCheckedMulI32(i32 a, i32 b, i32 *out)
{
    return !__builtin_mul_overflow(a, b, out);
}

/** @brief Computes `a / b` for @ref i32, checked for division by zero. */
static inline bool TemplateCheckedDivI32(i32 a, i32 b, i32 *out)
{
    if (b == 0)
        return false;

    *out = a / b;
    return true;
}

/** @brief Computes `a %% b` for @ref i32, checked for modulo by zero. */
static inline bool TemplateCheckedModI32(i32 a, i32 b, i32 *out)
{
    if (b == 0)
        return false;

    *out = a % b;
    return true;
}

/* --------------------------------------------------------------- i64 */

/** @brief Computes `a + b` for @ref i64, checked for overflow. */
static inline bool TemplateCheckedAddI64(i64 a, i64 b, i64 *out)
{
    return !__builtin_add_overflow(a, b, out);
}

/** @brief Computes `a - b` for @ref i64, checked for overflow. */
static inline bool TemplateCheckedSubI64(i64 a, i64 b, i64 *out)
{
    return !__builtin_sub_overflow(a, b, out);
}

/** @brief Computes `a * b` for @ref i64, checked for overflow. */
static inline bool TemplateCheckedMulI64(i64 a, i64 b, i64 *out)
{
    return !__builtin_mul_overflow(a, b, out);
}

/** @brief Computes `a / b` for @ref i64, checked for division by zero. */
static inline bool TemplateCheckedDivI64(i64 a, i64 b, i64 *out)
{
    if (b == 0)
        return false;

    *out = a / b;
    return true;
}

/** @brief Computes `a %% b` for @ref i64, checked for modulo by zero. */
static inline bool TemplateCheckedModI64(i64 a, i64 b, i64 *out)
{
    if (b == 0)
        return false;

    *out = a % b;
    return true;
}

/* ---------------------------------------------------------------- u8 */

/** @brief Computes `a + b` for @ref u8, checked for overflow. */
static inline bool TemplateCheckedAddU8(u8 a, u8 b, u8 *out)
{
    return !__builtin_add_overflow(a, b, out);
}

/**
 * @brief Computes `a - b` for @ref u8, checked for unsigned underflow.
 * @note `b > a` wraps around rather than invoking UB (unsigned type);
 *       that wraparound is treated as failure here.
 */
static inline bool TemplateCheckedSubU8(u8 a, u8 b, u8 *out)
{
    return !__builtin_sub_overflow(a, b, out);
}

/** @brief Computes `a * b` for @ref u8, checked for overflow. */
static inline bool TemplateCheckedMulU8(u8 a, u8 b, u8 *out)
{
    return !__builtin_mul_overflow(a, b, out);
}

/** @brief Computes `a / b` for @ref u8, checked for division by zero. */
static inline bool TemplateCheckedDivU8(u8 a, u8 b, u8 *out)
{
    if (b == 0)
        return false;

    *out = (u8)(a / b);
    return true;
}

/** @brief Computes `a %% b` for @ref u8, checked for modulo by zero. */
static inline bool TemplateCheckedModU8(u8 a, u8 b, u8 *out)
{
    if (b == 0)
        return false;

    *out = (u8)(a % b);
    return true;
}

/* --------------------------------------------------------------- u16 */

/** @brief Computes `a + b` for @ref u16, checked for overflow. */
static inline bool TemplateCheckedAddU16(u16 a, u16 b, u16 *out)
{
    return !__builtin_add_overflow(a, b, out);
}

/**
 * @brief Computes `a - b` for @ref u16, checked for unsigned underflow.
 * @note `b > a` wraps around rather than invoking UB (unsigned type);
 *       that wraparound is treated as failure here.
 */
static inline bool TemplateCheckedSubU16(u16 a, u16 b, u16 *out)
{
    return !__builtin_sub_overflow(a, b, out);
}

/** @brief Computes `a * b` for @ref u16, checked for overflow. */
static inline bool TemplateCheckedMulU16(u16 a, u16 b, u16 *out)
{
    return !__builtin_mul_overflow(a, b, out);
}

/** @brief Computes `a / b` for @ref u16, checked for division by zero. */
static inline bool TemplateCheckedDivU16(u16 a, u16 b, u16 *out)
{
    if (b == 0)
        return false;

    *out = (u16)(a / b);
    return true;
}

/** @brief Computes `a %% b` for @ref u16, checked for modulo by zero. */
static inline bool TemplateCheckedModU16(u16 a, u16 b, u16 *out)
{
    if (b == 0)
        return false;

    *out = (u16)(a % b);
    return true;
}

/* --------------------------------------------------------------- u32 */

/** @brief Computes `a + b` for @ref u32, checked for overflow. */
static inline bool TemplateCheckedAddU32(u32 a, u32 b, u32 *out)
{
    return !__builtin_add_overflow(a, b, out);
}

/**
 * @brief Computes `a - b` for @ref u32, checked for unsigned underflow.
 * @note `b > a` wraps around rather than invoking UB (unsigned type);
 *       that wraparound is treated as failure here.
 */
static inline bool TemplateCheckedSubU32(u32 a, u32 b, u32 *out)
{
    return !__builtin_sub_overflow(a, b, out);
}

/** @brief Computes `a * b` for @ref u32, checked for overflow. */
static inline bool TemplateCheckedMulU32(u32 a, u32 b, u32 *out)
{
    return !__builtin_mul_overflow(a, b, out);
}

/** @brief Computes `a / b` for @ref u32, checked for division by zero. */
static inline bool TemplateCheckedDivU32(u32 a, u32 b, u32 *out)
{
    if (b == 0)
        return false;

    *out = a / b;
    return true;
}

/** @brief Computes `a %% b` for @ref u32, checked for modulo by zero. */
static inline bool TemplateCheckedModU32(u32 a, u32 b, u32 *out)
{
    if (b == 0)
        return false;

    *out = a % b;
    return true;
}

/* --------------------------------------------------------------- u64 */

/** @brief Computes `a + b` for @ref u64, checked for overflow. */
static inline bool TemplateCheckedAddU64(u64 a, u64 b, u64 *out)
{
    return !__builtin_add_overflow(a, b, out);
}

/**
 * @brief Computes `a - b` for @ref u64, checked for unsigned underflow.
 * @note `b > a` wraps around rather than invoking UB (unsigned type);
 *       that wraparound is treated as failure here.
 */
static inline bool TemplateCheckedSubU64(u64 a, u64 b, u64 *out)
{
    return !__builtin_sub_overflow(a, b, out);
}

/** @brief Computes `a * b` for @ref u64, checked for overflow. */
static inline bool TemplateCheckedMulU64(u64 a, u64 b, u64 *out)
{
    return !__builtin_mul_overflow(a, b, out);
}

/** @brief Computes `a / b` for @ref u64, checked for division by zero. */
static inline bool TemplateCheckedDivU64(u64 a, u64 b, u64 *out)
{
    if (b == 0)
        return false;

    *out = a / b;
    return true;
}

/** @brief Computes `a %% b` for @ref u64, checked for modulo by zero. */
static inline bool TemplateCheckedModU64(u64 a, u64 b, u64 *out)
{
    if (b == 0)
        return false;

    *out = a % b;
    return true;
}

/* ------------------------------------------------------------- usize */

/** @brief Computes `a + b` for @ref usize, checked for overflow. */
static inline bool TemplateCheckedAddUsize(usize a, usize b, usize *out)
{
    return !__builtin_add_overflow(a, b, out);
}

/**
 * @brief Computes `a - b` for @ref usize, checked for unsigned underflow.
 * @note `b > a` wraps around rather than invoking UB (unsigned type);
 *       that wraparound is treated as failure here.
 */
static inline bool TemplateCheckedSubUsize(usize a, usize b, usize *out)
{
    return !__builtin_sub_overflow(a, b, out);
}

/**
 * @brief Computes `a * b` for @ref usize, checked for overflow.
 *
 * Primary intended use: validating `count * elemSize`-style allocation
 * sizes before calling into an actual allocator (see `TemplateAllocMany`
 * in the memory module).
 */
static inline bool TemplateCheckedMulUsize(usize a, usize b, usize *out)
{
    return !__builtin_mul_overflow(a, b, out);
}

/** @brief Computes `a / b` for @ref usize, checked for division by zero. */
static inline bool TemplateCheckedDivUsize(usize a, usize b, usize *out)
{
    if (b == 0)
        return false;

    *out = a / b;
    return true;
}

/** @brief Computes `a %% b` for @ref usize, checked for modulo by zero. */
static inline bool TemplateCheckedModUsize(usize a, usize b, usize *out)
{
    if (b == 0)
        return false;

    *out = a % b;
    return true;
}

/* ---------------------------------------------------------------- f32 */

/**
 * @brief Computes `a + b` for @ref f32, checked for overflow-to-infinity
 *        and NaN production.
 *
 * @note Unlike the integer `Checked*` functions, this cannot use a
 *       compiler overflow-detection intrinsic — `__builtin_add_overflow`
 *       and friends only accept integral types. Instead, the operation
 *       is performed and then the *result* is inspected: if the result
 *       is `inf`/`nan` but neither input already was, the operation
 *       itself produced that value, which is treated as failure. An
 *       `inf`/`nan` that was already present in an input (and simply
 *       propagates through) is not treated as a new failure — the
 *       caller presumably already knows about it.
 */
static inline bool TemplateCheckedAddF32(f32 a, f32 b, f32 *out)
{
    f32 result = a + b;

    if (isnan(result) && !isnan(a) && !isnan(b))
        return false;
    if (isinf(result) && !isinf(a) && !isinf(b))
        return false;

    *out = result;
    return true;
}

/** @brief Computes `a - b` for @ref f32, checked the same way as @ref TemplateCheckedAddF32. */
static inline bool TemplateCheckedSubF32(f32 a, f32 b, f32 *out)
{
    f32 result = a - b;

    if (isnan(result) && !isnan(a) && !isnan(b))
        return false;
    if (isinf(result) && !isinf(a) && !isinf(b))
        return false;

    *out = result;
    return true;
}

/** @brief Computes `a * b` for @ref f32, checked the same way as @ref TemplateCheckedAddF32. */
static inline bool TemplateCheckedMulF32(f32 a, f32 b, f32 *out)
{
    f32 result = a * b;

    if (isnan(result) && !isnan(a) && !isnan(b))
        return false;
    if (isinf(result) && !isinf(a) && !isinf(b))
        return false;

    *out = result;
    return true;
}

/**
 * @brief Computes `a / b` for @ref f32, checked for division by zero,
 *        overflow-to-infinity, and NaN production.
 *
 * @note A zero divisor is rejected outright (mirroring the integer
 *       `CheckedDiv*` functions), rather than being allowed to produce
 *       IEEE-754 infinity/NaN and then be caught by the post-hoc
 *       `isinf`/`isnan` check below — both amount to the same rejection,
 *       but checking `b == 0` first avoids relying on the following
 *       checks to catch it and documents the zero-divisor case
 *       explicitly.
 */
static inline bool TemplateCheckedDivF32(f32 a, f32 b, f32 *out)
{
    if (b == 0.0f)
        return false;

    f32 result = a / b;

    if (isnan(result) && !isnan(a) && !isnan(b))
        return false;
    if (isinf(result) && !isinf(a) && !isinf(b))
        return false;

    *out = result;
    return true;
}

/* ---------------------------------------------------------------- f64 */

/** @brief Computes `a + b` for @ref f64, checked the same way as @ref TemplateCheckedAddF32. */
static inline bool TemplateCheckedAddF64(f64 a, f64 b, f64 *out)
{
    f64 result = a + b;

    if (isnan(result) && !isnan(a) && !isnan(b))
        return false;
    if (isinf(result) && !isinf(a) && !isinf(b))
        return false;

    *out = result;
    return true;
}

/** @brief Computes `a - b` for @ref f64, checked the same way as @ref TemplateCheckedAddF32. */
static inline bool TemplateCheckedSubF64(f64 a, f64 b, f64 *out)
{
    f64 result = a - b;

    if (isnan(result) && !isnan(a) && !isnan(b))
        return false;
    if (isinf(result) && !isinf(a) && !isinf(b))
        return false;

    *out = result;
    return true;
}

/** @brief Computes `a * b` for @ref f64, checked the same way as @ref TemplateCheckedAddF32. */
static inline bool TemplateCheckedMulF64(f64 a, f64 b, f64 *out)
{
    f64 result = a * b;

    if (isnan(result) && !isnan(a) && !isnan(b))
        return false;
    if (isinf(result) && !isinf(a) && !isinf(b))
        return false;

    *out = result;
    return true;
}

/** @brief Computes `a / b` for @ref f64, checked the same way as @ref TemplateCheckedDivF32. */
static inline bool TemplateCheckedDivF64(f64 a, f64 b, f64 *out)
{
    if (b == 0.0)
        return false;

    f64 result = a / b;

    if (isnan(result) && !isnan(a) && !isnan(b))
        return false;
    if (isinf(result) && !isinf(a) && !isinf(b))
        return false;

    *out = result;
    return true;
}

#endif