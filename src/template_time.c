#define _POSIX_C_SOURCE 200809L /* for clock_gettime/CLOCK_MONOTONIC; must \
                                    precede every include below, since     \
                                    glibc feature-test macros only take    \
                                    effect if defined before the first     \
                                    system header that checks them */
#include <time.h>

#include "template_time.h"
#include "template_log.h"

TemplateInstant TemplateInstantNow(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (TemplateInstant){.sec = (i64)ts.tv_sec, .nsec = (i64)ts.tv_nsec};
}

f64 TemplateInstantDiffSeconds(TemplateInstant start, TemplateInstant end)
{
    i64 secDiff = end.sec - start.sec;
    i64 nsecDiff = end.nsec - start.nsec;

    if (nsecDiff < 0)
    {
        secDiff -= 1;
        nsecDiff += 1000000000LL;
    }

    return (f64)secDiff + (f64)nsecDiff / 1e9;
}

f64 TemplateInstantElapsedSeconds(TemplateInstant start)
{
    return TemplateInstantDiffSeconds(start, TemplateInstantNow());
}

static bool WallClockFormatted(
    char *buf,
    usize bufSize,
    const char *fmt,
    struct tm *(*convert)(const time_t *),
    char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE])
{
    time_t now = time(NULL);
    struct tm *tm = convert(&now);

    if (tm == NULL)
    {
        if (!TemplateSetErrorErrno(errorBuffer, "Failed to convert current time"))
            LOG_WARN("Error message truncated while reporting time-conversion failure");
        return false;
    }

    usize written = strftime(buf, bufSize, fmt, tm);
    if (written == 0)
    {
        if (!TemplateSetError(errorBuffer, "Formatted time did not fit in %zu-byte buffer", bufSize))
            LOG_WARN("Error message truncated while reporting strftime buffer failure");
        return false;
    }

    return true;
}

bool TemplateTryWallClockLocal(
    char *buf,
    usize bufSize,
    const char *fmt,
    char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE])
{
    return WallClockFormatted(buf, bufSize, fmt, localtime, errorBuffer);
}

bool TemplateTryWallClockUTC(
    char *buf,
    usize bufSize,
    const char *fmt,
    char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE])
{
    return WallClockFormatted(buf, bufSize, fmt, gmtime, errorBuffer);
}