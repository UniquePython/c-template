#ifndef TEMPLATE_DYN_ARRAY_TYPED_H_
#define TEMPLATE_DYN_ARRAY_TYPED_H_

/**
 * @file template_dyn_array_typed.h
 * @brief Generates a typed, compiler-checked wrapper around
 *        TemplateDynArray for a specific element type.
 *
 * Every function generated here is a thin, one-line forward to the
 * raw layer. There is no growth, bounds-checking, or allocation logic
 * here -- all of that is written exactly once in template_dyn_array.c,
 * and every instantiation below just inherits it.
 *
 * Usage:
 *
 *     TEMPLATE_DEFINE_DYN_ARRAY(i32, I32)
 *
 * generates a distinct type `TemplateDynArrayI32` plus the full
 * `TemplateDynArrayI32*` function family (TryNew, New, TryPush,
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
#include "template_dyn_array.h"
#include "template_types.h"

#define TEMPLATE_DEFINE_DYN_ARRAY(T, Name)                                                             \
    typedef struct                                                                                     \
    {                                                                                                  \
        TemplateDynArray *raw;                                                                         \
    } TemplateDynArray##Name;                                                                          \
                                                                                                       \
    /* Constructs an empty buffer, reporting failure via errorBuffer                                   \
     * instead of aborting. See TemplateDynArrayTryNew. */                                             \
    TEMPLATE_WARN_UNUSED_RESULT static inline bool TemplateDynArray##Name##TryNew(                     \
        TemplateDynArray##Name *out,                                                                   \
        char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE])                                         \
    {                                                                                                  \
        if (out == NULL)                                                                               \
            return false;                                                                              \
                                                                                                       \
        return TemplateDynArrayTryNew(&out->raw, sizeof(T), errorBuffer);                              \
    }                                                                                                  \
                                                                                                       \
    /* Constructs an empty buffer, aborting on failure. See                                            \
     * TemplateDynArrayNew. */                                                                         \
    TEMPLATE_WARN_UNUSED_RESULT static inline TemplateDynArray##Name TemplateDynArray##Name##New(void) \
    {                                                                                                  \
        return (TemplateDynArray##Name){.raw = TemplateDynArrayNew(sizeof(T))};                        \
    }                                                                                                  \
                                                                                                       \
    /* See TemplateDynArrayTryReserve. */                                                              \
    TEMPLATE_WARN_UNUSED_RESULT static inline bool TemplateDynArray##Name##TryReserve(                 \
        TemplateDynArray##Name *self,                                                                  \
        usize newCap,                                                                                  \
        char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE])                                         \
    {                                                                                                  \
        if (self == NULL)                                                                              \
            return false;                                                                              \
                                                                                                       \
        return TemplateDynArrayTryReserve(self->raw, newCap, errorBuffer);                             \
    }                                                                                                  \
                                                                                                       \
    /* Appends elem by value. See TemplateDynArrayTryPush. */                                          \
    TEMPLATE_WARN_UNUSED_RESULT static inline bool TemplateDynArray##Name##TryPush(                    \
        TemplateDynArray##Name *self,                                                                  \
        T elem,                                                                                        \
        char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE])                                         \
    {                                                                                                  \
        if (self == NULL)                                                                              \
            return false;                                                                              \
                                                                                                       \
        return TemplateDynArrayTryPush(self->raw, &elem, errorBuffer);                                 \
    }                                                                                                  \
                                                                                                       \
    /* Copies the element at index into *out. See                                                      \
     * TemplateDynArrayTryGet. */                                                                      \
    TEMPLATE_WARN_UNUSED_RESULT static inline bool TemplateDynArray##Name##TryGet(                     \
        const TemplateDynArray##Name *restrict self,                                                   \
        usize index,                                                                                   \
        T *restrict out)                                                                               \
    {                                                                                                  \
        if (self == NULL)                                                                              \
            return false;                                                                              \
                                                                                                       \
        return TemplateDynArrayTryGet(self->raw, index, out);                                          \
    }                                                                                                  \
                                                                                                       \
    /* See TemplateDynArrayLen. */                                                                     \
    static inline usize TemplateDynArray##Name##Len(const TemplateDynArray##Name *self)                \
    {                                                                                                  \
        if (self == NULL)                                                                              \
            return 0;                                                                                  \
                                                                                                       \
        return TemplateDynArrayLen(self->raw);                                                         \
    }                                                                                                  \
                                                                                                       \
    /* Removes and copies out the last element. out may be NULL. See                                   \
     * TemplateDynArrayTryPop. */                                                                      \
    TEMPLATE_WARN_UNUSED_RESULT static inline bool TemplateDynArray##Name##TryPop(                     \
        TemplateDynArray##Name *restrict self,                                                         \
        T *restrict out)                                                                               \
    {                                                                                                  \
        if (self == NULL)                                                                              \
            return false;                                                                              \
                                                                                                       \
        return TemplateDynArrayTryPop(self->raw, out);                                                 \
    }                                                                                                  \
                                                                                                       \
    /* Order-preserving removal. out may be NULL. See                                                  \
     * TemplateDynArrayTryRemove. */                                                                   \
    TEMPLATE_WARN_UNUSED_RESULT static inline bool TemplateDynArray##Name##TryRemove(                  \
        TemplateDynArray##Name *restrict self,                                                         \
        usize index,                                                                                   \
        T *restrict out)                                                                               \
    {                                                                                                  \
        if (self == NULL)                                                                              \
            return false;                                                                              \
                                                                                                       \
        return TemplateDynArrayTryRemove(self->raw, index, out);                                       \
    }                                                                                                  \
                                                                                                       \
    /* O(1), non-order-preserving removal. out may be NULL. See                                        \
     * TemplateDynArrayTrySwapRemove. */                                                               \
    TEMPLATE_WARN_UNUSED_RESULT static inline bool TemplateDynArray##Name##TrySwapRemove(              \
        TemplateDynArray##Name *restrict self,                                                         \
        usize index,                                                                                   \
        T *restrict out)                                                                               \
    {                                                                                                  \
        if (self == NULL)                                                                              \
            return false;                                                                              \
                                                                                                       \
        return TemplateDynArrayTrySwapRemove(self->raw, index, out);                                   \
    }                                                                                                  \
                                                                                                       \
    /* See TemplateDynArrayClear. */                                                                   \
    static inline void TemplateDynArray##Name##Clear(TemplateDynArray##Name *self)                     \
    {                                                                                                  \
        if (self == NULL)                                                                              \
            return;                                                                                    \
                                                                                                       \
        TemplateDynArrayClear(self->raw);                                                              \
    }                                                                                                  \
                                                                                                       \
    /* See TemplateDynArrayTryShrinkToFit. */                                                          \
    TEMPLATE_WARN_UNUSED_RESULT static inline bool TemplateDynArray##Name##TryShrinkToFit(             \
        TemplateDynArray##Name *self,                                                                  \
        char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE])                                         \
    {                                                                                                  \
        if (self == NULL)                                                                              \
            return false;                                                                              \
                                                                                                       \
        return TemplateDynArrayTryShrinkToFit(self->raw, errorBuffer);                                 \
    }                                                                                                  \
                                                                                                       \
    /* Frees the underlying buffer and nulls self->raw. See                                            \
     * TemplateDynArrayFree. */                                                                        \
    static inline void TemplateDynArray##Name##Free(TemplateDynArray##Name *self)                      \
    {                                                                                                  \
        if (self == NULL)                                                                              \
            return;                                                                                    \
                                                                                                       \
        TemplateDynArrayFree(&self->raw);                                                              \
    }

#endif