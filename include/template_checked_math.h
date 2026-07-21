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
 *
 * IMPLEMENTATION NOTE: every typed function is generated from the
 * X-macro bodies below rather than hand-written per type. `T` is the
 * underlying C type (e.g. `i32`) and `Suffix` is the PascalCase token
 * pasted into the function name (e.g. `I32`), so
 * `TEMPLATE_DEFINE_CHECKED_INT(i32, I32)` expands to
 * `TemplateCheckedAddI32`, `TemplateCheckedSubI32`, etc. This means a
 * doc comment can describe the *pattern* a macro generates, but not one
 * specific type's name — see each `TEMPLATE_*_BODY` macro below for
 * what it generates. (A C comment is stripped in translation phase 3,
 * before macro expansion/token-pasting runs in phase 4, so a comment
 * cannot itself have `T`/`Suffix` pasted into it the way a function
 * name can.)
 */

#include "template_types.h"

/* ------------------------------------------------------------------ */
/* Integer ops                                                         */
/* ------------------------------------------------------------------ */

/**
 * @brief Generates `TemplateCheckedAdd<Suffix>(T a, T b, T *out)`,
 *        computing `a + b` for the given integer type, checked for
 *        overflow via `__builtin_add_overflow`.
 */
#define TEMPLATE_INT_ADD_BODY(T, Suffix)                            \
    static inline bool TemplateCheckedAdd##Suffix(T a, T b, T *out) \
    {                                                               \
        return !__builtin_add_overflow(a, b, out);                  \
    }

/**
 * @brief Generates `TemplateCheckedSub<Suffix>(T a, T b, T *out)`,
 *        computing `a - b` for the given integer type, checked for
 *        overflow via `__builtin_sub_overflow`.
 */
#define TEMPLATE_INT_SUB_BODY(T, Suffix)                            \
    static inline bool TemplateCheckedSub##Suffix(T a, T b, T *out) \
    {                                                               \
        return !__builtin_sub_overflow(a, b, out);                  \
    }

/**
 * @brief Generates `TemplateCheckedMul<Suffix>(T a, T b, T *out)`,
 *        computing `a * b` for the given integer type, checked for
 *        overflow via `__builtin_mul_overflow`.
 */
#define TEMPLATE_INT_MUL_BODY(T, Suffix)                            \
    static inline bool TemplateCheckedMul##Suffix(T a, T b, T *out) \
    {                                                               \
        return !__builtin_mul_overflow(a, b, out);                  \
    }

/**
 * @brief Generates `TemplateCheckedDiv<Suffix>(T a, T b, T *out)`,
 *        computing `a / b` for the given integer type, checked for
 *        division by zero (there is no overflow intrinsic for
 *        division; unsigned division cannot overflow and this project
 *        does not special-case signed `MIN / -1`).
 */
#define TEMPLATE_INT_DIV_BODY(T, Suffix)                            \
    static inline bool TemplateCheckedDiv##Suffix(T a, T b, T *out) \
    {                                                               \
        if (b == 0)                                                 \
            return false;                                           \
                                                                    \
        *out = (T)(a / b);                                          \
        return true;                                                \
    }

/**
 * @brief Generates `TemplateCheckedMod<Suffix>(T a, T b, T *out)`,
 *        computing `a %% b` for the given integer type, checked for
 *        modulo by zero.
 */
#define TEMPLATE_INT_MOD_BODY(T, Suffix)                            \
    static inline bool TemplateCheckedMod##Suffix(T a, T b, T *out) \
    {                                                               \
        if (b == 0)                                                 \
            return false;                                           \
                                                                    \
        *out = (T)(a % b);                                          \
        return true;                                                \
    }

/**
 * @brief Expands to all five checked integer operations (add, sub,
 *        mul, div, mod) for one `(T, Suffix)` pair. This is the only
 *        macro invoked per type below — add a new integer width by
 *        adding one line here, not five function bodies.
 */
#define TEMPLATE_DEFINE_CHECKED_INT(T, Suffix) \
    TEMPLATE_INT_ADD_BODY(T, Suffix)           \
    TEMPLATE_INT_SUB_BODY(T, Suffix)           \
    TEMPLATE_INT_MUL_BODY(T, Suffix)           \
    TEMPLATE_INT_DIV_BODY(T, Suffix)           \
    TEMPLATE_INT_MOD_BODY(T, Suffix)

TEMPLATE_DEFINE_CHECKED_INT(i8, I8)
TEMPLATE_DEFINE_CHECKED_INT(i16, I16)
TEMPLATE_DEFINE_CHECKED_INT(i32, I32)
TEMPLATE_DEFINE_CHECKED_INT(i64, I64)
TEMPLATE_DEFINE_CHECKED_INT(u8, U8)
TEMPLATE_DEFINE_CHECKED_INT(u16, U16)
TEMPLATE_DEFINE_CHECKED_INT(u32, U32)
TEMPLATE_DEFINE_CHECKED_INT(u64, U64)
TEMPLATE_DEFINE_CHECKED_INT(usize, Usize)

#undef TEMPLATE_DEFINE_CHECKED_INT
#undef TEMPLATE_INT_ADD_BODY
#undef TEMPLATE_INT_SUB_BODY
#undef TEMPLATE_INT_MUL_BODY
#undef TEMPLATE_INT_DIV_BODY
#undef TEMPLATE_INT_MOD_BODY

/* ------------------------------------------------------------------ */
/* Floating-point ops                                                  */
/* ------------------------------------------------------------------ */

/**
 * @brief Generates `TemplateCheckedAdd<Suffix>(T a, T b, T *out)`,
 *        computing `a + b` for the given floating-point type. There is
 *        no overflow intrinsic for floats, so the result is computed
 *        and then classified: a new `inf`/`nan` in the result that was
 *        not already present in `a` or `b` is treated as failure.
 */
#define TEMPLATE_FLOAT_ADD_BODY(T, Suffix)                          \
    static inline bool TemplateCheckedAdd##Suffix(T a, T b, T *out) \
    {                                                               \
        T result = a + b;                                           \
        if (isnan(result) && !isnan(a) && !isnan(b))                \
            return false;                                           \
        if (isinf(result) && !isinf(a) && !isinf(b))                \
            return false;                                           \
                                                                    \
        *out = result;                                              \
        return true;                                                \
    }

/**
 * @brief Generates `TemplateCheckedSub<Suffix>(T a, T b, T *out)`,
 *        computing `a - b` for the given floating-point type, using
 *        the same post-hoc `inf`/`nan` classification described above
 *        `TEMPLATE_FLOAT_ADD_BODY`.
 */
#define TEMPLATE_FLOAT_SUB_BODY(T, Suffix)                          \
    static inline bool TemplateCheckedSub##Suffix(T a, T b, T *out) \
    {                                                               \
        T result = a - b;                                           \
        if (isnan(result) && !isnan(a) && !isnan(b))                \
            return false;                                           \
        if (isinf(result) && !isinf(a) && !isinf(b))                \
            return false;                                           \
                                                                    \
        *out = result;                                              \
        return true;                                                \
    }

/**
 * @brief Generates `TemplateCheckedMul<Suffix>(T a, T b, T *out)`,
 *        computing `a * b` for the given floating-point type, using
 *        the same post-hoc `inf`/`nan` classification described above
 *        `TEMPLATE_FLOAT_ADD_BODY`.
 */
#define TEMPLATE_FLOAT_MUL_BODY(T, Suffix)                          \
    static inline bool TemplateCheckedMul##Suffix(T a, T b, T *out) \
    {                                                               \
        T result = a * b;                                           \
        if (isnan(result) && !isnan(a) && !isnan(b))                \
            return false;                                           \
        if (isinf(result) && !isinf(a) && !isinf(b))                \
            return false;                                           \
                                                                    \
        *out = result;                                              \
        return true;                                                \
    }

/**
 * @brief Generates `TemplateCheckedDiv<Suffix>(T a, T b, T *out)`,
 *        computing `a / b` for the given floating-point type: a
 *        literal zero divisor is rejected up front, then the same
 *        post-hoc `inf`/`nan` classification described above
 *        `TEMPLATE_FLOAT_ADD_BODY` is applied to the result.
 */
#define TEMPLATE_FLOAT_DIV_BODY(T, Suffix)                          \
    static inline bool TemplateCheckedDiv##Suffix(T a, T b, T *out) \
    {                                                               \
        if (b == (T)0)                                              \
            return false;                                           \
                                                                    \
        T result = a / b;                                           \
        if (isnan(result) && !isnan(a) && !isnan(b))                \
            return false;                                           \
        if (isinf(result) && !isinf(a) && !isinf(b))                \
            return false;                                           \
                                                                    \
        *out = result;                                              \
        return true;                                                \
    }

/**
 * @brief Expands to all four checked floating-point operations (add,
 *        sub, mul, div) for one `(T, Suffix)` pair. Modulo is
 *        intentionally omitted for floating-point types.
 */
#define TEMPLATE_DEFINE_CHECKED_FLOAT(T, Suffix) \
    TEMPLATE_FLOAT_ADD_BODY(T, Suffix)           \
    TEMPLATE_FLOAT_SUB_BODY(T, Suffix)           \
    TEMPLATE_FLOAT_MUL_BODY(T, Suffix)           \
    TEMPLATE_FLOAT_DIV_BODY(T, Suffix)

TEMPLATE_DEFINE_CHECKED_FLOAT(f32, F32)
TEMPLATE_DEFINE_CHECKED_FLOAT(f64, F64)

#undef TEMPLATE_DEFINE_CHECKED_FLOAT
#undef TEMPLATE_FLOAT_ADD_BODY
#undef TEMPLATE_FLOAT_SUB_BODY
#undef TEMPLATE_FLOAT_MUL_BODY
#undef TEMPLATE_FLOAT_DIV_BODY

#endif