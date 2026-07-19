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
    {
        if (!TemplateSetError(errorBuffer, "Cannot allocate 0 bytes"))
            LOG_WARN("Error message was truncated");
        return false;
    }

    void *ptr = malloc(size);
    if (ptr == NULL)
    {
        if (!TemplateSetErrorErrno(errorBuffer, "Failed to allocate " USIZE_FMT " bytes", size))
            LOG_WARN("Error message was truncated");
        return false;
    }

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
    {
        if (!TemplateSetError(errorBuffer, "Allocation size overflow: " USIZE_FMT " * " USIZE_FMT, count, elemSize))
            LOG_WARN("Error message was truncated");
        return false;
    }

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
    {
        if (!TemplateSetError(errorBuffer, "Cannot allocate 0 bytes"))
            LOG_WARN("Error message was truncated");
        return false;
    }

    void *newPtr = realloc(*ptr, newSize);
    if (newPtr == NULL)
    {
        if (!TemplateSetErrorErrno(errorBuffer, "Failed to reallocate to " USIZE_FMT " bytes", newSize))
            LOG_WARN("Error message was truncated");
        free(*ptr);
        *ptr = NULL;
        return false;
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
