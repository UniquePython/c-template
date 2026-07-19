#include "template_memory.h"
#include "template_checked_math.h"
#include "template_log.h"

#include <stdlib.h>
#include <string.h>

bool TemplateTryAlloc(
    void **out,
    usize size,
    char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE])
{
    if (out == NULL)
        return false;

    if (size == 0)
        return TemplateSetError(errorBuffer, "Cannot allocate 0 bytes");

    void *ptr = malloc(size);
    if (ptr == NULL)
        return TemplateSetErrorErrno(errorBuffer, "Failed to allocate %zu bytes", size);

    memset(ptr, 0, size);

    *out = ptr;
    return true;
}

void *TemplateAlloc(usize size)
{
    void *ptr;
    TEMPLATE_NEW_ERROR_BUFFER(errorBuffer);

    if (!TemplateTryAlloc(&ptr, size, errorBuffer))
        LOG_FATAL("%s", errorBuffer);

    return ptr;
}

bool TemplateTryAllocMany(
    void **out,
    usize count,
    usize elemSize,
    char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE])
{
    if (out == NULL)
        return false;

    usize size;
    if (!TemplateCheckedMulUsize(count, elemSize, &size))
        return TemplateSetError(errorBuffer, "Allocation size overflow: %zu * %zu", count, elemSize);

    return TemplateTryAlloc(out, size, errorBuffer);
}

void *TemplateAllocMany(usize count, usize elemSize)
{
    void *ptr;
    TEMPLATE_NEW_ERROR_BUFFER(errorBuffer);

    if (!TemplateTryAllocMany(&ptr, count, elemSize, errorBuffer))
        LOG_FATAL("%s", errorBuffer);

    return ptr;
}

bool TemplateTryRealloc(
    void **ptr,
    usize newSize,
    char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE])
{
    if (ptr == NULL)
        return false;

    if (newSize == 0)
        return TemplateSetError(errorBuffer, "Cannot allocate 0 bytes");

    void *newPtr = realloc(*ptr, newSize);
    if (newPtr == NULL)
    {
        bool ok = TemplateSetErrorErrno(errorBuffer, "Failed to reallocate to %zu bytes", newSize);
        free(*ptr);
        *ptr = NULL;
        return ok;
    }

    *ptr = newPtr;
    return true;
}

void TemplateRealloc(void **ptr, usize newSize)
{
    TEMPLATE_NEW_ERROR_BUFFER(errorBuffer);

    if (!TemplateTryRealloc(ptr, newSize, errorBuffer))
        LOG_FATAL("%s", errorBuffer);
}

void TemplateFree(void **ptr)
{
    if (ptr == NULL || *ptr == NULL)
        return;

    free(*ptr);
    *ptr = NULL;
}
