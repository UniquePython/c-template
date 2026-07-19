#include "template_error.h"
#include "template_log.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

bool TemplateWarnIfErrorSet(char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE])
{
    if (errorBuffer == NULL)
        return false;

    if (TEMPLATE_ERROR_BUFFER_HAS_ERROR(errorBuffer))
    {
        LOG_WARN("Error buffer already contains a message and may be overwritten: \"%s\"", errorBuffer);
        return true;
    }

    return false;
}

bool TemplateSetError(
    char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE],
    const char *restrict fmt,
    ...)
{
    if (errorBuffer == NULL || fmt == NULL)
        return false;

    TemplateWarnIfErrorSet(errorBuffer);

    va_list args;
    va_start(args, fmt);
    int written = vsnprintf(errorBuffer, TEMPLATE_ERROR_BUFFER_SIZE, fmt, args);
    va_end(args);

    return written >= 0 && (usize)written < TEMPLATE_ERROR_BUFFER_SIZE;
}

bool TemplatePropagateError(
    char outerBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE],
    const char innerBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE],
    const char *restrict fmt,
    ...)
{
    if (outerBuffer == NULL || innerBuffer == NULL || fmt == NULL)
        return false;

    TemplateWarnIfErrorSet(outerBuffer);

    char context[TEMPLATE_ERROR_BUFFER_SIZE];

    va_list args;
    va_start(args, fmt);
    int written = vsnprintf(context, sizeof(context), fmt, args);
    va_end(args);

    if (written < 0)
        return false;

    bool truncated = (usize)written >= sizeof(context);

    if (TEMPLATE_ERROR_BUFFER_HAS_ERROR(innerBuffer))
    {
        int total = snprintf(outerBuffer, TEMPLATE_ERROR_BUFFER_SIZE, "%s: %s", context, innerBuffer);
        if (total < 0)
            return false;
        truncated = truncated || (usize)total >= TEMPLATE_ERROR_BUFFER_SIZE;
    }
    else
    {
        memcpy(outerBuffer, context, sizeof(context));
    }

    return !truncated;
}

bool TemplateSetErrorErrno(
    char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE],
    const char *restrict fmt,
    ...)
{
    if (errorBuffer == NULL || fmt == NULL)
        return false;

    TemplateWarnIfErrorSet(errorBuffer);

    int savedErrno = errno;

    char context[TEMPLATE_ERROR_BUFFER_SIZE];

    va_list args;
    va_start(args, fmt);
    int written = vsnprintf(context, sizeof(context), fmt, args);
    va_end(args);

    if (written < 0)
        return false;

    bool truncated = (usize)written >= sizeof(context);

    int total = snprintf(errorBuffer, TEMPLATE_ERROR_BUFFER_SIZE, "%s: %s", context, strerror(savedErrno));
    if (total < 0)
        return false;

    truncated = truncated || (usize)total >= TEMPLATE_ERROR_BUFFER_SIZE;

    return !truncated;
}
