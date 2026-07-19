#ifndef TEMPLATE_ERROR_H_
#define TEMPLATE_ERROR_H_

#include "template_attributes.h"

#include "template_types.h"

#ifndef TEMPLATE_ERROR_BUFFER_SIZE
#define TEMPLATE_ERROR_BUFFER_SIZE 2048
#endif

void TemplateSetError(
    char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE],
    const char *restrict fmt,
    ...) TEMPLATE_PRINTF(2, 3);

#define TEMPLATE_ERROR_BUFFER(name) char name[TEMPLATE_ERROR_BUFFER_SIZE] = {0}
#define TEMPLATE_CLEAR_ERROR_BUFFER(name) ((name)[0] = '\0')

#endif
