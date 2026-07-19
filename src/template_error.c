#include "template_error.h"

#include <stdarg.h>
#include <stdio.h>

void TemplateSetError(
    char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE],
    const char *restrict fmt,
    ...)
{
    if (errorBuffer == NULL || fmt == NULL)
        return;

    va_list args;
    va_start(args, fmt);
    vsnprintf(errorBuffer, TEMPLATE_ERROR_BUFFER_SIZE, fmt, args);
    va_end(args);
}
