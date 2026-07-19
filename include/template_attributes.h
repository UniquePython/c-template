#ifndef TEMPLATE_ATTRIBUTES_H_
#define TEMPLATE_ATTRIBUTES_H_

/**
 * @file template_attributes.h
 * @brief Portable wrappers around GCC/Clang function attributes used
 *        across the project.
 *
 * These macros exist so call sites and declarations don't spell out
 * `__attribute__((...))` directly. Requires GCC or Clang — there is no
 * fallback definition for other compilers, so code using these macros
 * will not compile under e.g. MSVC.
 */

/**
 * @def TEMPLATE_PRINTF(fmtIndex, argIndex)
 * @brief Marks a variadic function as printf-like so the compiler checks
 *        the format string against the supplied arguments.
 *
 * @param fmtIndex 1-based index (counting from the function's first
 *                 declared parameter) of the `const char *` format-string
 *                 parameter.
 * @param argIndex 1-based index of the first variadic argument (typically
 *                 `fmtIndex + 1`).
 *
 * Enables `-Wformat`-class warnings for calls to the annotated function,
 * catching mismatches like `%d` against a `char *` argument at compile
 * time.
 *
 * @code
 * void LogInfo(const char *fmt, ...) TEMPLATE_PRINTF(1, 2);
 * @endcode
 */
#define TEMPLATE_PRINTF(fmtIndex, argIndex) __attribute__((format(printf, fmtIndex, argIndex)))

/**
 * @def TEMPLATE_CONST
 * @brief Marks a function as depending only on its arguments, with no
 *        reads of global or pointed-to memory and no observable side
 *        effects.
 *
 * Stricter than @ref TEMPLATE_PURE: a `const` function may not even
 * dereference pointer arguments to read memory — its result must be a
 * pure function of the argument *values* alone. This allows the compiler
 * to freely reorder, cache, or eliminate redundant calls.
 *
 * @warning Applying this to a function that reads through a pointer
 *          argument, touches a global, or calls a non-const function is
 *          undefined behavior — the compiler is permitted to assume this
 *          contract holds and may miscompile code that violates it. Only
 *          use on functions with no pointer/array parameters and no
 *          access to anything beyond their by-value arguments.
 */
#define TEMPLATE_CONST __attribute__((const))

/**
 * @def TEMPLATE_PURE
 * @brief Marks a function as free of observable side effects, but
 *        (unlike @ref TEMPLATE_CONST) permitted to read global or
 *        pointed-to memory.
 *
 * A pure function's return value may depend on the current contents of
 * memory it reads (globals, `*ptr`), but it must not write to memory
 * visible to the caller, perform I/O, or otherwise affect anything
 * beyond its return value. This still allows the compiler to elide
 * redundant calls as long as the read-from memory hasn't changed between
 * calls.
 *
 * @warning As with @ref TEMPLATE_CONST, violating this contract (e.g. by
 *          writing to a global or through a pointer argument) is
 *          undefined behavior, not merely a missed optimization.
 */
#define TEMPLATE_PURE __attribute__((pure))

/**
 * @def TEMPLATE_WARN_UNUSED_RESULT
 * @brief Warns if the caller discards the annotated function's return
 *        value.
 *
 * Intended for functions whose return value signals something the
 * caller needs to act on (e.g. success/failure, truncation) rather than
 * being purely informational. This is a compiler *warning*, not an
 * error — this project builds with `-Wall -Wextra` but not `-Werror`, so
 * an ignored result will be visible in build output but will not by
 * itself fail the build.
 *
 * A caller that genuinely intends to discard the result can silence the
 * warning with an explicit `(void)` cast on the call expression.
 */
#define TEMPLATE_WARN_UNUSED_RESULT __attribute__((warn_unused_result))

/**
 * @def TEMPLATE_NORETURN
 * @brief Declares that a function never returns control to its caller
 *        (e.g. it always calls `abort()`, `exit()`, or an infinite loop).
 *
 * Lets the compiler and static analysis (e.g. `-fanalyzer`) reason
 * correctly about control flow past a call to the annotated function —
 * for example, suppressing false "used uninitialized" warnings on code
 * that is unreachable after the call.
 *
 * @warning This is a hard promise to the compiler: if a function marked
 *          `TEMPLATE_NORETURN` returns on any code path (including
 *          falling off the end of the function body), the result is
 *          undefined behavior, not a diagnosable error.
 */
#define TEMPLATE_NORETURN __attribute__((noreturn))

/**
 * @def TEMPLATE_DISCARD_CONST(ptr)
 * @brief Casts away `const` (or `volatile`) from a pointer without
 *        triggering a `-Wcast-qual` warning.
 *
 * This project builds with `-Wcast-qual`, so a plain `(void *)ptr` cast
 * on a `const`-qualified pointer would warn project-wide. This macro
 * scopes the diagnostic suppression to just the one cast, via a
 * statement-expression that saves the pointer, performs the cast inside
 * a local `#pragma GCC diagnostic` push/pop, and evaluates to the
 * resulting `void *`.
 *
 * @param ptr A pointer expression (may be `const`- and/or
 *            `volatile`-qualified).
 * @return The same pointer value, as `void *`, with qualifiers dropped.
 *
 * @warning This macro only removes the *type-level* `const` qualifier
 *          from the pointer. It does not change whether the pointee
 *          itself was originally declared `const`. Writing through the
 *          result is only actually defined behavior if the underlying
 *          object is genuinely mutable (e.g. it's a non-`const` object
 *          you're holding via a `const`-qualified pointer/parameter).
 *          Writing through this cast to an object that was truly
 *          declared `const` is undefined behavior, regardless of the
 *          cast.
 * @note Requires GCC/Clang statement-expression (`__extension__({ ... })`)
 *       support; not portable to strictly-conforming compilers.
 */
#define TEMPLATE_DISCARD_CONST(ptr)                                                                                                       \
    (__extension__({                                                                                                                      \
        __typeof__(ptr) TEMPLATE_DISCARD_CONST_tmp_ = (ptr);                                                                              \
        _Pragma("GCC diagnostic push")                                                                                                    \
            _Pragma("GCC diagnostic ignored \"-Wcast-qual\"") void *TEMPLATE_DISCARD_CONST_result_ = (void *)TEMPLATE_DISCARD_CONST_tmp_; \
        _Pragma("GCC diagnostic pop")                                                                                                     \
            TEMPLATE_DISCARD_CONST_result_;                                                                                               \
    }))

/**
 * @def TEMPLATE_UNUSED_VAR(var)
 * @brief Explicitly marks a variable as intentionally unused, silencing
 *        `-Wunused-variable`/`-Wunused-parameter`.
 *
 * @param var The variable (or parameter) to mark as used.
 *
 * Equivalent to writing `(void)(var);` directly, but gives the
 * suppression a searchable, self-documenting name — grepping for
 * `TEMPLATE_UNUSED_VAR` finds every deliberate unused-variable site,
 * whereas grepping for bare `(void)` casts is far noisier.
 */
#define TEMPLATE_UNUSED_VAR(var) (void)(var)

#endif