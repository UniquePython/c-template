#ifndef TEMPLATE_ERROR_H_
#define TEMPLATE_ERROR_H_

#include "template_attributes.h"

#include "template_types.h"

#ifndef TEMPLATE_ERROR_BUFFER_SIZE
/*
 * TEMPLATE_ERROR_BUFFER_SIZE must be defined identically (or not at all) in every translation unit
 * that either calls `TemplateSetError` or compiles `template_error.c`, since it affects the compiled
 * function body itself, not just the caller's local buffer declaration.
 */
#define TEMPLATE_ERROR_BUFFER_SIZE 2048
#endif

bool TemplateWarnIfErrorSet(char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE]);

TEMPLATE_WARN_UNUSED_RESULT bool TemplateSetError(
    char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE],
    const char *restrict fmt,
    ...) TEMPLATE_PRINTF(2, 3);

TEMPLATE_WARN_UNUSED_RESULT bool TemplatePropagateError(
    char outerBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE],
    const char innerBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE],
    const char *restrict fmt,
    ...) TEMPLATE_PRINTF(3, 4);

TEMPLATE_WARN_UNUSED_RESULT bool TemplateSetErrorErrno(
    char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE],
    const char *restrict fmt,
    ...) TEMPLATE_PRINTF(2, 3);

#define TEMPLATE_NEW_ERROR_BUFFER(name) char name[TEMPLATE_ERROR_BUFFER_SIZE] = {0}
#define TEMPLATE_CLEAR_ERROR_BUFFER(name) (name)[0] = '\0'
#define TEMPLATE_ERROR_BUFFER_HAS_ERROR(name) ((name)[0] != '\0')

#endif
