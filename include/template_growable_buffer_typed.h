#ifndef TEMPLATE_GROWABLE_BUFFER_TYPED_H_
#define TEMPLATE_GROWABLE_BUFFER_TYPED_H_

/**
 * @file template_growable_buffer_typed.h
 * @brief Generates a typed, compiler-checked wrapper around
 *        TemplateGrowableBuffer for a specific element type.
 *
 * Every function generated here is a thin, one-line forward to the
 * raw layer. There is no growth, bounds-checking, or allocation logic
 * here -- all of that is written exactly once in template_growable_buffer.c,
 * and every instantiation below just inherits it.
 *
 * Usage:
 *
 *     TEMPLATE_DEFINE_GROWABLE_BUFFER(i32, I32)
 *
 * generates a distinct type `TemplateGrowableBufferI32` plus the full
 * `TemplateGrowableBufferI32*` function family (TryNew, New, TryPush,
 * TryGet, ...), all `static inline`. Safe to invoke this macro in
 * multiple translation units for the same (T, Name) pair -- `static
 * inline` gives each TU its own copy, which the compiler is expected
 * to fold away given these are trivial forwarding calls.
 *
 * Do NOT invoke this macro twice with the same `Name` but a different
 * `T` in code that's ever compiled together -- the generated type name
 * only depends on `Name`, so that would be a redefinition, not two
 * distinct types.
 *
 * There is deliberately no direct/unchecked indexed access generated
 * here (no `vec.data[i]`-equivalent) -- every read goes through
 * TryGet, which is bounds-checked.
 */

#include "template_attributes.h"
#include "template_error.h"
#include "template_growable_buffer.h"
#include "template_types.h"

#define TEMPLATE_DEFINE_GROWABLE_BUFFER(T, Name)                                                                   \
    typedef struct                                                                                                 \
    {                                                                                                              \
        TemplateGrowableBuffer *raw;                                                                               \
    } TemplateGrowableBuffer##Name;                                                                                \
                                                                                                                   \
    /* Constructs an empty buffer, reporting failure via errorBuffer                                               \
     * instead of aborting. See TemplateGrowableBufferTryNew. */                                                   \
    TEMPLATE_WARN_UNUSED_RESULT static inline bool TemplateGrowableBuffer##Name##TryNew(                           \
        TemplateGrowableBuffer##Name *out,                                                                         \
        char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE])                                                     \
    {                                                                                                              \
        if (out == NULL)                                                                                           \
            return false;                                                                                          \
                                                                                                                   \
        return TemplateGrowableBufferTryNew(&out->raw, sizeof(T), errorBuffer);                                    \
    }                                                                                                              \
                                                                                                                   \
    /* Constructs an empty buffer, aborting on failure. See                                                        \
     * TemplateGrowableBufferNew. */                                                                               \
    TEMPLATE_WARN_UNUSED_RESULT static inline TemplateGrowableBuffer##Name TemplateGrowableBuffer##Name##New(void) \
    {                                                                                                              \
        return (TemplateGrowableBuffer##Name){.raw = TemplateGrowableBufferNew(sizeof(T))};                        \
    }                                                                                                              \
                                                                                                                   \
    /* See TemplateGrowableBufferTryReserve. */                                                                    \
    TEMPLATE_WARN_UNUSED_RESULT static inline bool TemplateGrowableBuffer##Name##TryReserve(                       \
        TemplateGrowableBuffer##Name *self,                                                                        \
        usize newCap,                                                                                              \
        char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE])                                                     \
    {                                                                                                              \
        if (self == NULL)                                                                                          \
            return false;                                                                                          \
                                                                                                                   \
        return TemplateGrowableBufferTryReserve(self->raw, newCap, errorBuffer);                                   \
    }                                                                                                              \
                                                                                                                   \
    /* Appends elem by value. See TemplateGrowableBufferTryPush. */                                                \
    TEMPLATE_WARN_UNUSED_RESULT static inline bool TemplateGrowableBuffer##Name##TryPush(                          \
        TemplateGrowableBuffer##Name *self,                                                                        \
        T elem,                                                                                                    \
        char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE])                                                     \
    {                                                                                                              \
        if (self == NULL)                                                                                          \
            return false;                                                                                          \
                                                                                                                   \
        return TemplateGrowableBufferTryPush(self->raw, &elem, errorBuffer);                                       \
    }                                                                                                              \
                                                                                                                   \
    /* Copies the element at index into *out. See                                                                  \
     * TemplateGrowableBufferTryGet. */                                                                            \
    TEMPLATE_WARN_UNUSED_RESULT static inline bool TemplateGrowableBuffer##Name##TryGet(                           \
        const TemplateGrowableBuffer##Name *self,                                                                  \
        usize index,                                                                                               \
        T *out)                                                                                                    \
    {                                                                                                              \
        if (self == NULL)                                                                                          \
            return false;                                                                                          \
                                                                                                                   \
        return TemplateGrowableBufferTryGet(self->raw, index, out);                                                \
    }                                                                                                              \
                                                                                                                   \
    /* See TemplateGrowableBufferLen. */                                                                           \
    static inline usize TemplateGrowableBuffer##Name##Len(const TemplateGrowableBuffer##Name *self)                \
    {                                                                                                              \
        if (self == NULL)                                                                                          \
            return 0;                                                                                              \
                                                                                                                   \
        return TemplateGrowableBufferLen(self->raw);                                                               \
    }                                                                                                              \
                                                                                                                   \
    /* Removes and copies out the last element. out may be NULL. See                                               \
     * TemplateGrowableBufferTryPop. */                                                                            \
    TEMPLATE_WARN_UNUSED_RESULT static inline bool TemplateGrowableBuffer##Name##TryPop(                           \
        TemplateGrowableBuffer##Name *self,                                                                        \
        T *out)                                                                                                    \
    {                                                                                                              \
        if (self == NULL)                                                                                          \
            return false;                                                                                          \
                                                                                                                   \
        return TemplateGrowableBufferTryPop(self->raw, out);                                                       \
    }                                                                                                              \
                                                                                                                   \
    /* Order-preserving removal. out may be NULL. See                                                              \
     * TemplateGrowableBufferTryRemove. */                                                                         \
    TEMPLATE_WARN_UNUSED_RESULT static inline bool TemplateGrowableBuffer##Name##TryRemove(                        \
        TemplateGrowableBuffer##Name *self,                                                                        \
        usize index,                                                                                               \
        T *out)                                                                                                    \
    {                                                                                                              \
        if (self == NULL)                                                                                          \
            return false;                                                                                          \
                                                                                                                   \
        return TemplateGrowableBufferTryRemove(self->raw, index, out);                                             \
    }                                                                                                              \
                                                                                                                   \
    /* O(1), non-order-preserving removal. out may be NULL. See                                                    \
     * TemplateGrowableBufferTrySwapRemove. */                                                                     \
    TEMPLATE_WARN_UNUSED_RESULT static inline bool TemplateGrowableBuffer##Name##TrySwapRemove(                    \
        TemplateGrowableBuffer##Name *self,                                                                        \
        usize index,                                                                                               \
        T *out)                                                                                                    \
    {                                                                                                              \
        if (self == NULL)                                                                                          \
            return false;                                                                                          \
                                                                                                                   \
        return TemplateGrowableBufferTrySwapRemove(self->raw, index, out);                                         \
    }                                                                                                              \
                                                                                                                   \
    /* See TemplateGrowableBufferClear. */                                                                         \
    static inline void TemplateGrowableBuffer##Name##Clear(TemplateGrowableBuffer##Name *self)                     \
    {                                                                                                              \
        if (self == NULL)                                                                                          \
            return;                                                                                                \
                                                                                                                   \
        TemplateGrowableBufferClear(self->raw);                                                                    \
    }                                                                                                              \
                                                                                                                   \
    /* See TemplateGrowableBufferTryShrinkToFit. */                                                                \
    TEMPLATE_WARN_UNUSED_RESULT static inline bool TemplateGrowableBuffer##Name##TryShrinkToFit(                   \
        TemplateGrowableBuffer##Name *self,                                                                        \
        char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE])                                                     \
    {                                                                                                              \
        if (self == NULL)                                                                                          \
            return false;                                                                                          \
                                                                                                                   \
        return TemplateGrowableBufferTryShrinkToFit(self->raw, errorBuffer);                                       \
    }                                                                                                              \
                                                                                                                   \
    /* Frees the underlying buffer and nulls self->raw. See                                                        \
     * TemplateGrowableBufferFree. */                                                                              \
    static inline void TemplateGrowableBuffer##Name##Free(TemplateGrowableBuffer##Name *self)                      \
    {                                                                                                              \
        if (self == NULL)                                                                                          \
            return;                                                                                                \
                                                                                                                   \
        TemplateGrowableBufferFree(&self->raw);                                                                    \
    }

#endif