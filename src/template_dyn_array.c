#include "template_growable_buffer.h"
#include "template_assert.h"
#include "template_checked_math.h"
#include "template_log.h"
#include "template_memory.h"

#include <string.h>

struct TemplateDynArray
{
    void *data;
    usize len;
    usize cap;
    usize elemSize;
};

bool TemplateDynArrayTryNew(
    TemplateDynArray **out,
    usize elemSize,
    char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE])
{
    if (out == NULL)
        return false;

    if (elemSize == 0)
    {
        if (!TemplateSetError(errorBuffer, "Cannot create a buffer with elemSize 0"))
            LOG_WARN("Error message was truncated");
        return false;
    }

    TemplateDynArray *buf;
    if (!TemplateTryAlloc((void **)&buf, sizeof *buf, errorBuffer))
        return false;

    /* TemplateTryAlloc already zero-initializes, so data/len/cap start
     * at NULL/0/0; only elemSize needs setting explicitly. */
    buf->elemSize = elemSize;

    *out = buf;
    return true;
}

TemplateDynArray *TemplateDynArrayNew(usize elemSize)
{
    TemplateDynArray *buf;
    TEMPLATE_NEW_ERROR_BUFFER(errorBuffer);

    if (!TemplateDynArrayTryNew(&buf, elemSize, errorBuffer))
        LOG_FATAL("%s", errorBuffer);

    return buf;
}

bool TemplateDynArrayTryReserve(
    TemplateDynArray *buf,
    usize newCap,
    char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE])
{
    if (buf == NULL)
        return false;

    TEMPLATE_ASSERT(buf->elemSize != 0, "TemplateDynArray with elemSize 0 -- should be unreachable, TryNew rejects this at construction");

    if (newCap <= buf->cap)
        return true;

    usize newSizeBytes;
    if (!TemplateCheckedMulUsize(newCap, buf->elemSize, &newSizeBytes))
    {
        if (!TemplateSetError(errorBuffer, "Growable buffer capacity overflow: " USIZE_FMT " * " USIZE_FMT, newCap, buf->elemSize))
            LOG_WARN("Error message was truncated");
        return false;
    }

    if (buf->data == NULL)
    {
        /* First-ever allocation: TemplateTryRealloc requires *ptr to
         * already be a valid (possibly NULL, per realloc's own
         * contract) pointer, but we want the zero-init TemplateTryAlloc
         * gives us rather than realloc's uninitialized memory on this
         * initial allocation specifically. */
        void *newData;
        if (!TemplateTryAlloc(&newData, newSizeBytes, errorBuffer))
            return false;

        buf->data = newData;
        buf->cap = newCap;
        return true;
    }

    /* TemplateTryRealloc frees *ptr and sets it to NULL on failure --
     * it never leaves the old block leaked, but it also means buf->data
     * is already gone by the time we'd report failure here. That's a
     * deliberate, documented tradeoff of TemplateTryRealloc itself, so
     * TemplateDynArrayTryReserve inherits it rather than hiding
     * it: a failed reserve leaves buf with len/cap reset to reflect
     * that the storage is gone, not silently pretending the old,
     * smaller buffer is still usable. */
    void *newData = buf->data;
    if (!TemplateTryRealloc(&newData, newSizeBytes, errorBuffer))
    {
        buf->data = NULL;
        buf->len = 0;
        buf->cap = 0;
        return false;
    }

    buf->data = newData;
    buf->cap = newCap;
    return true;
}

bool TemplateDynArrayTryPush(
    TemplateDynArray *buf,
    const void *elem,
    char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE])
{
    if (buf == NULL || elem == NULL)
        return false;

    if (buf->len == buf->cap)
    {
        usize newCap = (buf->cap == 0) ? 1 : buf->cap * 2;

        /* buf->cap * 2 above is intentionally not the overflow-checked
         * path -- TemplateDynArrayTryReserve re-validates
         * newCap * elemSize immediately below regardless, so a
         * doubling that wrapped usize would simply fail the reserve's
         * own overflow check rather than silently succeed. */
        if (!TemplateDynArrayTryReserve(buf, newCap, errorBuffer))
            return false;
    }

    memcpy((char *)buf->data + (buf->len * buf->elemSize), elem, buf->elemSize);
    buf->len++;
    return true;
}

bool TemplateDynArrayTryGet(
    const TemplateDynArray *restrict buf,
    usize index,
    void *restrict out)
{
    if (buf == NULL || out == NULL)
        return false;

    if (index >= buf->len)
        return false;

    memcpy(out, (const char *)buf->data + (index * buf->elemSize), buf->elemSize);
    return true;
}

usize TemplateDynArrayLen(const TemplateDynArray *buf)
{
    if (buf == NULL)
        return 0;

    return buf->len;
}

bool TemplateDynArrayTryPop(
    TemplateDynArray *restrict buf,
    void *restrict out)
{
    if (buf == NULL || buf->len == 0)
        return false;

    buf->len--;

    if (out != NULL)
        memcpy(out, (const char *)buf->data + (buf->len * buf->elemSize), buf->elemSize);

    return true;
}

bool TemplateDynArrayTryRemove(
    TemplateDynArray *restrict buf,
    usize index,
    void *restrict out)
{
    if (buf == NULL)
        return false;

    if (index >= buf->len)
        return false;

    if (out != NULL)
        memcpy(out, (const char *)buf->data + (index * buf->elemSize), buf->elemSize);

    usize tailCount = buf->len - index - 1;
    if (tailCount > 0)
    {
        void *dst = (char *)buf->data + (index * buf->elemSize);
        const void *src = (const char *)buf->data + ((index + 1) * buf->elemSize);
        memmove(dst, src, tailCount * buf->elemSize);
    }

    buf->len--;
    return true;
}

bool TemplateDynArrayTrySwapRemove(
    TemplateDynArray *restrict buf,
    usize index,
    void *restrict out)
{
    if (buf == NULL)
        return false;

    if (index >= buf->len)
        return false;

    void *slot = (char *)buf->data + (index * buf->elemSize);

    if (out != NULL)
        memcpy(out, slot, buf->elemSize);

    usize lastIndex = buf->len - 1;
    if (index != lastIndex)
    {
        const void *last = (const char *)buf->data + (lastIndex * buf->elemSize);
        memcpy(slot, last, buf->elemSize);
    }

    buf->len--;
    return true;
}

void TemplateDynArrayClear(TemplateDynArray *buf)
{
    if (buf == NULL)
        return;

    buf->len = 0;
}

bool TemplateDynArrayTryShrinkToFit(
    TemplateDynArray *buf,
    char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE])
{
    if (buf == NULL)
        return false;

    if (buf->len == buf->cap)
        return true;

    if (buf->len == 0)
    {
        TemplateFree(&buf->data);
        buf->cap = 0;
        return true;
    }

    usize newSizeBytes;
    if (!TemplateCheckedMulUsize(buf->len, buf->elemSize, &newSizeBytes))
    {
        /* Unreachable in practice -- buf->len <= buf->cap always, and
         * cap * elemSize already fit in usize when it was reserved. */
        TEMPLATE_ASSERT(false, "buf->len * buf->elemSize overflowed usize during shrink -- implies a corrupted TemplateDynArray");
    }

    void *newData = buf->data;
    if (!TemplateTryRealloc(&newData, newSizeBytes, errorBuffer))
    {
        /* A failed shrink to a SMALLER size is not the same risk as a
         * failed growth -- most realloc implementations do not fail
         * when shrinking. If it somehow does, TemplateTryRealloc has
         * already freed buf->data (see the note in TryReserve above),
         */
        buf->data = NULL;
        buf->len = 0;
        buf->cap = 0;
        return false;
    }

    buf->data = newData;
    buf->cap = buf->len;
    return true;
}

void TemplateDynArrayFree(TemplateDynArray **buf)
{
    if (buf == NULL || *buf == NULL)
        return;

    TemplateFree(&(*buf)->data);
    TemplateFree((void **)buf);
}