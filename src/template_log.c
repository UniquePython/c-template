#define _POSIX_C_SOURCE 200809L

#include "template_log.h"
#include "template_log_internal.h"

#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

static bool gUseColorStdout = false;
static bool gUseColorStderr = false;

#define TEMPLATE_COLOR_FATAL TEMPLATE_COLOR_REVERSE TEMPLATE_COLOR_RED
#define TEMPLATE_COLOR_DEPRECATED TEMPLATE_COLOR_REVERSE TEMPLATE_COLOR_MAGENTA
#define TEMPLATE_COLOR_NOT_IMPLEMENTED TEMPLATE_COLOR_REVERSE TEMPLATE_COLOR_YELLOW

void TemplateLogInit(void)
{
    gUseColorStdout = isatty(fileno(stdout));
    gUseColorStderr = isatty(fileno(stderr));
}

// Writes one log line: "[LEVEL] message (file:line, in func)".
//
// The message always comes first so it has visual priority; the location is
// a secondary, dimmed annex meant for when the reader actually needs to jump
// to the source, not for every-glance scanning. Pass file == NULL to omit
// the location entirely (used by TemplateLogInfo).
void TemplateLogWrite(
    FILE *restrict stream,
    bool useColor,
    bool flush,
    const char *restrict level,
    const char *restrict color,
    const char *restrict file,
    i32 line,
    const char *restrict func,
    const char *restrict fmt,
    va_list args)
{
    if (useColor)
        fprintf(stream, "%s%s[%s]%s ", TEMPLATE_COLOR_BOLD, color, level, TEMPLATE_COLOR_RESET);
    else
        fprintf(stream, "[%s] ", level);

    vfprintf(stream, fmt, args);

    if (file != NULL)
    {
        if (useColor)
            fprintf(stream, " %s(%s:%d, in %s)%s", TEMPLATE_COLOR_GRAY, file, line, func, TEMPLATE_COLOR_RESET);
        else
            fprintf(stream, " (%s:%d, in %s)", file, line, func);
    }

    fprintf(stream, "\n");

    if (flush)
        fflush(stream);
}

bool TemplateLogStderrColorEnabled(void)
{
    return gUseColorStderr;
}

#if TEMPLATE_VERBOSITY >= 1
void TemplateLogFatal(const char *restrict file, i32 line, const char *restrict func, const char *restrict fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TemplateLogWrite(stderr, gUseColorStderr, true, "FATAL", TEMPLATE_COLOR_FATAL, file, line, func, fmt, args);
    va_end(args);

    abort();
}

void TemplateLogNotImplemented(const char *restrict file, i32 line, const char *restrict func, const char *restrict fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TemplateLogWrite(stderr, gUseColorStderr, true, "NOT IMPLEMENTED", TEMPLATE_COLOR_NOT_IMPLEMENTED, file, line, func, fmt, args);
    va_end(args);

    abort();
}
#endif

#if TEMPLATE_VERBOSITY >= 2
void TemplateLogError(const char *restrict file, i32 line, const char *restrict func, const char *restrict fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TemplateLogWrite(stderr, gUseColorStderr, true, "ERROR", TEMPLATE_COLOR_RED, file, line, func, fmt, args);
    va_end(args);
}
#endif

#if TEMPLATE_VERBOSITY >= 3
void TemplateLogWarning(const char *restrict file, i32 line, const char *restrict func, const char *restrict fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TemplateLogWrite(stdout, gUseColorStdout, false, "WARNING", TEMPLATE_COLOR_MAGENTA, file, line, func, fmt, args);
    va_end(args);
}

void TemplateLogDeprecated(const char *restrict file, i32 line, const char *restrict func, const char *restrict fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TemplateLogWrite(stdout, gUseColorStdout, false, "DEPRECATED", TEMPLATE_COLOR_DEPRECATED, file, line, func, fmt, args);
    va_end(args);
}
#endif

#if TEMPLATE_VERBOSITY >= 4
void TemplateLogInfo(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TemplateLogWrite(stdout, gUseColorStdout, false, "INFO", TEMPLATE_COLOR_GREEN, NULL, 0, NULL, fmt, args);
    va_end(args);
}
#endif

#if TEMPLATE_VERBOSITY >= 5
void TemplateLogDebug(const char *restrict file, i32 line, const char *restrict func, const char *restrict fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TemplateLogWrite(stdout, gUseColorStdout, false, "DEBUG", TEMPLATE_COLOR_YELLOW, file, line, func, fmt, args);
    va_end(args);
}
#endif

#if TEMPLATE_VERBOSITY >= 6
void TemplateLogTrace(const char *restrict file, i32 line, const char *restrict func, const char *restrict fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TemplateLogWrite(stdout, gUseColorStdout, false, "TRACE", TEMPLATE_COLOR_CYAN, file, line, func, fmt, args);
    va_end(args);
}
#endif
