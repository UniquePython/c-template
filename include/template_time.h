#ifndef TEMPLATE_TIME_H_
#define TEMPLATE_TIME_H_

#include "template_types.h"
#include "template_error.h"

/* ---------------------------------------------------------- monotonic */

/**
 * @brief Opaque monotonic time point, unaffected by system clock changes
 *        (NTP adjustments, manual clock changes, DST). Only ever
 *        meaningful relative to another TemplateInstant from the same
 *        process run — never compare across processes or persist it.
 */
typedef struct
{
    i64 sec;
    i64 nsec;
} TemplateInstant;

/** @brief Captures the current monotonic time. Cannot fail in practice. */
TemplateInstant TemplateInstantNow(void);

/**
 * @brief Seconds elapsed between two instants (`end - start`).
 *
 * @note Correctly handles the case where `end`'s nanosecond component is
 *       smaller than `start`'s (i.e. a whole-second boundary was
 *       crossed), by borrowing a second rather than producing a
 *       negative fractional part.
 */
f64 TemplateInstantDiffSeconds(TemplateInstant start, TemplateInstant end);

/**
 * @brief Seconds elapsed between `start` and now. Shorthand for
 *        `TemplateInstantDiffSeconds(start, TemplateInstantNow())`.
 */
f64 TemplateInstantElapsedSeconds(TemplateInstant start);

/* ---------------------------------------------------------- wall clock */

/**
 * @brief Formats the current local wall-clock time into `buf` using a
 *        strftime-style format string.
 *
 * @note Uses non-thread-safe `localtime()` internally, per project
 *       convention — safe here because the resulting `struct tm*` is
 *       consumed by `strftime` and never returned to the caller, so no
 *       caller can observe it being overwritten by a later call. Do not
 *       extend this file with a function that returns a `struct tm*` or
 *       `struct tm` populated from `localtime`/`gmtime` without
 *       reconsidering this.
 * @note Returns false if `strftime` reports the formatted result did
 *       not fit in `buf` (per the ISO C spec, `strftime` returns 0 in
 *       that case, unlike `snprintf`'s truncation-count behavior) — the
 *       buffer's contents are then unspecified and must not be used.
 */
bool TemplateTryWallClockLocal(
    char *restrict buf,
    usize bufSize,
    const char *restrict fmt,
    char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE]);

/** @brief Same as @ref TemplateTryWallClockLocal but in UTC (via `gmtime`). */
bool TemplateTryWallClockUTC(
    char *restrict buf,
    usize bufSize,
    const char *restrict fmt,
    char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE]);

#endif /* TEMPLATE_TIME_H */
