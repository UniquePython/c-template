#ifndef TEMPLATE_DYN_ARRAY_H_
#define TEMPLATE_DYN_ARRAY_H_

/**
 * @file template_growable_buffer.h
 * @brief Generic, type-erased growable array (the storage core).
 *
 * This is one real implementation of growth, bounds-checking, and
 * allocation, operating on `void *` storage plus an `elemSize` recorded
 * at creation time. It is not meant to be used directly at most call
 * sites -- see the TEMPLATE_DEFINE_DYN_ARRAY macro,
 * which wraps this in a typed, compiler-checked shell per element type,
 * so the growth/overflow logic below is written exactly once and every
 * typed instantiation just forwards to it.
 *
 * TemplateDynArray is opaque and always heap-allocated -- callers
 * only ever hold a `TemplateDynArray *`, obtained from
 * @ref TemplateDynArrayNew or @ref TemplateDynArrayTryNew,
 * and released via @ref TemplateDynArrayFree. There is no way to
 * construct one on the stack, and no field of the struct is part of the
 * public API.
 *
 * Naming convention: every function that can fail for a reason the
 * caller can meaningfully react to is named TemplateDynArrayTryX
 * and reports failure via a `bool` return plus the standard
 * error-buffer convention (see `template_error.h`) -- consistent with
 * `template_memory.h`. Failures here are only ever "the outside world
 * said no" (allocation failure, overflow, an out-of-bounds index passed
 * in from outside). Broken internal invariants (e.g. `len > cap`,
 * which should be structurally impossible if this file's own logic is
 * correct) are TEMPLATE_ASSERT, not a Try-reported failure -- see
 * template_assert.h.
 *
 * All growth arithmetic is overflow-checked via `template_checked_math.h`
 * before it ever reaches an allocator call, and all allocation goes
 * through `template_memory.h`'s Try-family, so a TemplateDynArray
 * can never be pushed into a state that silently wraps `cap * elemSize`
 * into a too-small allocation.
 *
 * @note A failed growth (TryReserve, or a TryPush that had to grow)
 *       does not leave the buffer at its previous, smaller size --
 *       it is reset to empty (no data, len 0, cap 0). This follows
 *       directly from `TemplateTryRealloc`'s own contract, which frees
 *       the existing block before reporting a failed reallocation
 *       rather than leaving two copies of the pointer to reason about.
 *       There is no partial/rolled-back state to recover into; the
 *       buffer is safe to keep using (e.g. push into again from empty)
 *       but the pre-failure contents are gone.
 */

#include "template_attributes.h"
#include "template_error.h"
#include "template_types.h"

/**
 * @brief Opaque, type-erased growable array. See file-level docs.
 */
typedef struct TemplateDynArray TemplateDynArray;

/**
 * @brief Constructs an empty buffer for elements of `elemSize` bytes,
 *        reporting failure via `errorBuffer` instead of aborting.
 *
 * @param[out] out Set to a pointer to the new buffer on success. Left
 *                 unmodified on failure.
 * @param elemSize Size of one element, in bytes. Must be nonzero.
 * @param errorBuffer Error buffer to report failure into.
 * @return `true` on success, `false` if `elemSize == 0` or the struct
 *         itself couldn't be allocated. No backing storage is allocated
 *         at this point either way -- only the (small, fixed-size)
 *         struct itself.
 */
TEMPLATE_WARN_UNUSED_RESULT bool TemplateDynArrayTryNew(
    TemplateDynArray **out,
    usize elemSize,
    char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE]);

/**
 * @brief Constructs an empty buffer for elements of `elemSize` bytes,
 *        aborting on failure.
 *
 * @param elemSize Size of one element, in bytes. Must be nonzero.
 * @return A pointer to the new buffer. Never returns `NULL` -- logs
 *         `FATAL` and aborts if `elemSize == 0` or the struct itself
 *         couldn't be allocated.
 */
TEMPLATE_WARN_UNUSED_RESULT TemplateDynArray *TemplateDynArrayNew(usize elemSize);

/**
 * @brief Ensures capacity for at least `newCap` elements, growing the
 *        backing allocation if necessary.
 *
 * @param[in,out] buf The buffer to reserve capacity on.
 * @param newCap Minimum total capacity (in elements, not bytes) the
 *               buffer should have after this call. If `newCap` is not
 *               greater than the buffer's current capacity, this is a
 *               no-op that always succeeds.
 * @param errorBuffer Error buffer to report failure into.
 * @return `true` on success (including the no-op case), `false` if
 *         `newCap * elemSize` overflows `usize` or the underlying
 *         allocation/reallocation failed. See the file-level note on
 *         what state `buf` is left in after a failed growth.
 */
TEMPLATE_WARN_UNUSED_RESULT bool TemplateDynArrayTryReserve(
    TemplateDynArray *buf,
    usize newCap,
    char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE]);

/**
 * @brief Appends a copy of `*elem` (`elemSize` bytes, as given to
 *        @ref TemplateDynArrayNew) to the end of the buffer,
 *        growing the backing allocation if necessary.
 *
 * @param[in,out] buf The buffer to push onto.
 * @param elem Pointer to the element to copy in. Must not be `NULL`.
 * @param errorBuffer Error buffer to report failure into.
 * @return `true` on success, `false` if growth was needed and failed.
 *         See the file-level note on what state `buf` is left in after
 *         a failed growth.
 *
 * Growth strategy: capacity doubles (starting from an initial capacity
 * of 1 on the first push into an empty buffer) whenever the buffer is
 * full, via @ref TemplateDynArrayTryReserve.
 */
TEMPLATE_WARN_UNUSED_RESULT bool TemplateDynArrayTryPush(
    TemplateDynArray *buf,
    const void *elem,
    char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE]);

/**
 * @brief Copies the element at `index` into `*out`.
 *
 * @param buf The buffer to read from.
 * @param index Zero-based index. Must be less than the buffer's length.
 * @param[out] out Set to a copy of the element on success. Left
 *                 unmodified on failure.
 * @return `true` on success, `false` if `index` is out of bounds.
 *
 * Unlike push/reserve, an out-of-range `index` here is treated as a
 * normal, recoverable failure (not a TEMPLATE_ASSERT) -- the index
 * usually originates from outside this file (a loop bound, user input,
 * a computed offset), so it is a boundary check, not an internal
 * invariant.
 */
TEMPLATE_WARN_UNUSED_RESULT bool TemplateDynArrayTryGet(
    const TemplateDynArray *restrict buf,
    usize index,
    void *restrict out);

/**
 * @brief Returns the number of elements currently stored.
 *
 * @param buf The buffer to query.
 * @return The current length. Always `0` for a `NULL` `buf`.
 */
usize TemplateDynArrayLen(const TemplateDynArray *buf);

/**
 * @brief Removes and copies out the last element.
 *
 * @param[in,out] buf The buffer to pop from.
 * @param[out] out Set to a copy of the removed element on success. May
 *                 be `NULL` if the caller doesn't need the value.
 * @return `true` on success, `false` if the buffer is empty.
 *
 * Does not shrink the backing allocation -- capacity is unchanged, only
 * length decreases. Use @ref TemplateDynArrayTryShrinkToFit to
 * release unused capacity explicitly.
 */
TEMPLATE_WARN_UNUSED_RESULT bool TemplateDynArrayTryPop(
    TemplateDynArray *restrict buf,
    void *restrict out);

/**
 * @brief Removes the element at `index`, shifting all later elements
 *        left by one to fill the gap. Order-preserving.
 *
 * @param[in,out] buf The buffer to remove from.
 * @param index Zero-based index. Must be less than the buffer's length.
 * @param[out] out Set to a copy of the removed element on success. May
 *                 be `NULL` if the caller doesn't need the value.
 * @return `true` on success, `false` if `index` is out of bounds.
 *
 * O(n) in the number of elements after `index`. See @ref
 * TemplateDynArrayTrySwapRemove for an O(1) alternative when
 * element order doesn't need to be preserved.
 */
TEMPLATE_WARN_UNUSED_RESULT bool TemplateDynArrayTryRemove(
    TemplateDynArray *restrict buf,
    usize index,
    void *restrict out);

/**
 * @brief Removes the element at `index` in O(1) by moving the last
 *        element into its place. Does not preserve order.
 *
 * @param[in,out] buf The buffer to remove from.
 * @param index Zero-based index. Must be less than the buffer's length.
 * @param[out] out Set to a copy of the removed element on success. May
 *                 be `NULL` if the caller doesn't need the value.
 * @return `true` on success, `false` if `index` is out of bounds.
 */
TEMPLATE_WARN_UNUSED_RESULT bool TemplateDynArrayTrySwapRemove(
    TemplateDynArray *restrict buf,
    usize index,
    void *restrict out);

/**
 * @brief Resets the buffer's length to `0`. Does not free or shrink the
 *        backing allocation, and does not fail.
 *
 * @param[in,out] buf The buffer to clear.
 */
void TemplateDynArrayClear(TemplateDynArray *buf);

/**
 * @brief Shrinks the backing allocation to exactly fit the buffer's
 *        current length (or frees it entirely if the buffer is empty).
 *
 * @param[in,out] buf The buffer to shrink.
 * @param errorBuffer Error buffer to report failure into.
 * @return `true` on success (including the already-minimal no-op
 *         case), `false` if the underlying reallocation failed -- in
 *         which case, per the file-level note, `buf` is reset to
 *         empty rather than left at its pre-shrink size. A failed
 *         *shrink* specifically is rare in practice (most allocators
 *         do not fail when asked for less memory), but is handled the
 *         same way as any other failed TemplateTryRealloc call for
 *         consistency rather than special-cased.
 */
TEMPLATE_WARN_UNUSED_RESULT bool TemplateDynArrayTryShrinkToFit(
    TemplateDynArray *buf,
    char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE]);

/**
 * @brief Frees `*buf` (backing storage and the struct itself) and sets
 *        `*buf` to `NULL`.
 *
 * @param[in,out] buf Pointer to the pointer to free. If `buf` is `NULL`
 *                     or `*buf` is already `NULL`, this is a no-op
 *                     (matching @ref TemplateFree's behavior, which
 *                     this is implemented directly in terms of) -- a
 *                     second `TemplateDynArrayFree(&buf)` call is
 *                     a safe no-op rather than a double free.
 */
void TemplateDynArrayFree(TemplateDynArray **buf);

#endif