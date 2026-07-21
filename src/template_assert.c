#include "template_assert.h"
#include "template_log_internal.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef TEMPLATE_ASSERT_MSG_SIZE
/*
 * TEMPLATE_ASSERT_MSG_SIZE must be defined identically (or not at all) in every translation unit
 * that either calls `TEMPLATE_ASSERT` or compiles `template_assert.c`, since it affects the compiled
 * function body itself.
 */
#define TEMPLATE_ASSERT_MSG_SIZE 512
#endif

#define TEMPLATE_COLOR_ASSERT TEMPLATE_COLOR_REVERSE TEMPLATE_COLOR_RED

/*
 * TemplateLogWrite() takes one format string plus one va_list to
 * vfprintf against it. By the time we know condStr and the caller's
 * fully-formatted message, we're past the point of having a live
 * va_list for a *new* format string ("(%s) %s") -- va_start requires a
 * "..." parameter list on the function it's called in, and
 * TemplateAssertFail's own va_list has already been consumed by
 * vsnprintf below. This tiny variadic shim exists purely to open a
 * fresh va_list for that fixed two-string format and hand it to
 * TemplateLogWrite.
 */
static void TemplateAssertWriteLine(
    FILE *restrict stream, bool useColor, const char *restrict file,
    i32 line, const char *restrict func, const char *restrict fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TemplateLogWrite(stream, useColor, true, "ASSERT", TEMPLATE_COLOR_ASSERT, file, line, func, fmt, args);
    va_end(args);
}

/*
 * Unlike every other log level, this is NOT gated behind #if VERBOSITY
 * (or TEMPLATE_ASSERTIONS_DEBUG -- that only controls
 * TEMPLATE_ASSERT_DEBUG). TEMPLATE_ASSERT exists to catch broken
 * invariants -- bugs in the program's own logic -- and those are
 * exactly as real in a release build as in a debug one. If this could
 * be compiled away, release builds would silently lose the one
 * mechanism that turns "state I already know is corrupt" into an
 * immediate, diagnosable abort instead of undefined behavior somewhere
 * downstream.
 */
TEMPLATE_NORETURN void TemplateAssertFail(
    const char *file, i32 line, const char *func,
    const char *condStr, const char *fmt, ...)
{
    char message[TEMPLATE_ASSERT_MSG_SIZE];

    va_list args;
    va_start(args, fmt);
    vsnprintf(message, sizeof message, fmt, args);
    va_end(args);

    TemplateAssertWriteLine(stderr, TemplateLogStderrColorEnabled(), file, line, func, "(%s) %s", condStr, message);

    abort();
}