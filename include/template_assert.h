#ifndef TEMPLATE_ASSERT_H_
#define TEMPLATE_ASSERT_H_

#include "template_attributes.h"
#include "template_types.h"

#ifndef TEMPLATE_ASSERTIONS_DEBUG
/*
 * Controls TEMPLATE_ASSERT_DEBUG only. TEMPLATE_ASSERT is never gated --
 * see template_assert.c for why.
 *
 * 0 = TEMPLATE_ASSERT_DEBUG compiled out
 * 1 = TEMPLATE_ASSERT_DEBUG active (default in debug/dev configs)
 */
#define TEMPLATE_ASSERTIONS_DEBUG 1
#endif

/* Always compiled in, in every build configuration, including release.
 * Use for invariants your own code guarantees -- if this fires, it means
 * your program's logic is already wrong, not that a caller passed bad
 * input. See template_error.h / Try*-family functions for the latter. */
TEMPLATE_NORETURN void TemplateAssertFail(
    const char *file,
    i32 line,
    const char *func,
    const char *condStr,
    const char *fmt, ...) TEMPLATE_PRINTF(5, 6);

#define TEMPLATE_ASSERT(cond, ...)                                                \
    do                                                                            \
    {                                                                             \
        if (!(cond))                                                              \
            TemplateAssertFail(__FILE__, __LINE__, __func__, #cond, __VA_ARGS__); \
    } while (0)

#if TEMPLATE_ASSERTIONS_DEBUG
#define TEMPLATE_ASSERT_DEBUG(cond, ...) TEMPLATE_ASSERT(cond, __VA_ARGS__)
#else
#define TEMPLATE_ASSERT_DEBUG(cond, ...) ((void)0)
#endif

#endif