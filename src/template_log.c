#define _POSIX_C_SOURCE 200809L

#include "template_log.h"

#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

static bool gUseColorStdout = false;
static bool gUseColorStderr = false;

#define TEMPLATE_COLOR_RESET "\033[0m"
#define TEMPLATE_COLOR_BOLD "\033[1m"

#define TEMPLATE_COLOR_RED "\033[31m"
#define TEMPLATE_COLOR_MAGENTA "\033[35m"
#define TEMPLATE_COLOR_YELLOW "\033[33m"
#define TEMPLATE_COLOR_CYAN "\033[36m"
#define TEMPLATE_COLOR_GRAY "\033[90m"

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
static void TemplateLogWrite(
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

#if VERBOSITY >= 1
void TemplateLogError(const char *file, i32 line, const char *func, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TemplateLogWrite(stderr, gUseColorStderr, true, "ERROR", TEMPLATE_COLOR_RED, file, line, func, fmt, args);
    va_end(args);
}
#endif

#if VERBOSITY >= 2
void TemplateLogWarning(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TemplateLogWrite(stdout, gUseColorStdout, false, "WARNING", TEMPLATE_COLOR_MAGENTA, NULL, 0, NULL, fmt, args);
    va_end(args);
}
#endif

#if VERBOSITY >= 3
void TemplateLogInfo(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TemplateLogWrite(stdout, gUseColorStdout, false, "INFO", TEMPLATE_COLOR_YELLOW, NULL, 0, NULL, fmt, args);
    va_end(args);
}
#endif

#if VERBOSITY >= 4
void TemplateLogDebug(const char *file, i32 line, const char *func, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TemplateLogWrite(stdout, gUseColorStdout, false, "DEBUG", TEMPLATE_COLOR_CYAN, file, line, func, fmt, args);
    va_end(args);
}
#endif
