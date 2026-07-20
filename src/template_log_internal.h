#ifndef TEMPLATE_LOG_INTERNAL_H_
#define TEMPLATE_LOG_INTERNAL_H_

#include "template_types.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>

#define TEMPLATE_COLOR_RESET "\033[0m"
#define TEMPLATE_COLOR_BOLD "\033[1m"
#define TEMPLATE_COLOR_REVERSE "\033[7m"

#define TEMPLATE_COLOR_RED "\033[31m"
#define TEMPLATE_COLOR_MAGENTA "\033[35m"
#define TEMPLATE_COLOR_GREEN "\033[32m"
#define TEMPLATE_COLOR_YELLOW "\033[33m"
#define TEMPLATE_COLOR_CYAN "\033[36m"
#define TEMPLATE_COLOR_GRAY "\033[90m"

/* Internal to the template_log/template_assert pair. Not part of the public logging API */
void TemplateLogWrite(
    FILE *restrict stream, bool useColor, bool flush,
    const char *restrict level, const char *restrict color,
    const char *restrict file, i32 line, const char *restrict func,
    const char *restrict fmt, va_list args);

bool TemplateLogStderrColorEnabled(void);

#endif