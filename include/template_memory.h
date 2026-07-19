#ifndef TEMPLATE_MEMORY_H_
#define TEMPLATE_MEMORY_H_

/**
 * @file template_memory.h
 * @brief Checked heap allocation wrappers.
 *
 * Two families of functions are provided for each operation
 * (allocate / allocate-many / reallocate):
 *
 *   - `TemplateTryX` — never aborts. Reports failure via a `bool` return
 *     and the standard error-buffer convention (see `template_error.h`).
 *     Use this whenever OOM (or a rejected zero-size/overflowing
 *     request) is something the caller can meaningfully recover from.
 *
 *   - `TemplateX` — aborts (via `LOG_FATAL`) on any failure the `Try`
 *     variant would have reported. Never returns `NULL`. Use this for
 *     the common case where there is no real recovery path for
 *     allocation failure anyway, and you don't want to thread an error
 *     buffer through every call site.
 *
 * `TemplateX` is implemented directly in terms of the corresponding
 * `TemplateTryX`, so there is exactly one place the real allocation
 * logic lives for each operation.
 *
 * All allocations are zero-initialized (via an explicit `memset` after
 * the underlying `malloc`/`realloc` call — this project does not rely on
 * `calloc`, since `TemplateTryAllocMany`'s overflow-checked size
 * computation already replaces `calloc`'s own internal overflow check).
 *
 * `TemplateTryAllocMany`/`TemplateAllocMany` compute `count * elemSize`
 * via `TemplateCheckedMulUsize` (see `template_checked_math.h`) before
 * allocating, so a `count`/`elemSize` pair that would overflow `usize`
 * is rejected rather than silently wrapping into a too-small allocation
 * (the classic integer-overflow-into-heap-overflow bug).
 *
 * A request for zero bytes (directly, or as the result of
 * `count * elemSize == 0`) is rejected as an error in every function
 * below, rather than being passed through to the underlying allocator
 * (whose behavior for a zero-size request is implementation-defined).
 */

#include "template_attributes.h"
#include "template_error.h"
#include "template_types.h"

/**
 * @brief Allocates `size` zeroed bytes, reporting failure via
 *        `errorBuffer` instead of aborting.
 *
 * @param[out] out Set to a pointer to the new zeroed allocation on
 *                 success. Left unmodified on failure.
 * @param size Number of bytes to allocate. Must be nonzero.
 * @param errorBuffer Error buffer to report failure into (see
 *                     `template_error.h`).
 * @return `true` on success, `false` if `size == 0` or the underlying
 *         allocator failed (an error message is set in `errorBuffer` in
 *         either case).
 */
TEMPLATE_WARN_UNUSED_RESULT bool TemplateTryAlloc(
    void **out,
    usize size,
    char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE]);

/**
 * @brief Allocates `size` zeroed bytes, aborting on failure.
 *
 * @param size Number of bytes to allocate. Must be nonzero.
 * @return A pointer to the new zeroed allocation. Never returns `NULL`
 *         — logs `FATAL` (via `LOG_FATAL`) and aborts the process if
 *         `size == 0` or the underlying allocator fails.
 */
TEMPLATE_WARN_UNUSED_RESULT void *TemplateAlloc(usize size);

/**
 * @brief Allocates a zeroed array of `count` elements of `elemSize`
 *        bytes each, reporting failure via `errorBuffer` instead of
 *        aborting.
 *
 * @param[out] out Set to a pointer to the new zeroed allocation on
 *                 success. Left unmodified on failure.
 * @param count Number of elements.
 * @param elemSize Size of each element, in bytes.
 * @param errorBuffer Error buffer to report failure into.
 * @return `true` on success, `false` if `count * elemSize` overflows
 *         `usize`, if the resulting size is `0`, or if the underlying
 *         allocator failed.
 *
 * `count == 0` or `elemSize == 0` is not special-cased separately — it
 * is caught naturally when the resulting zero size reaches the same
 * zero-size rejection used by @ref TemplateTryAlloc.
 */
TEMPLATE_WARN_UNUSED_RESULT bool TemplateTryAllocMany(
    void **out,
    usize count,
    usize elemSize,
    char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE]);

/**
 * @brief Allocates a zeroed array of `count` elements of `elemSize`
 *        bytes each, aborting on failure.
 *
 * @param count Number of elements.
 * @param elemSize Size of each element, in bytes.
 * @return A pointer to the new zeroed allocation. Never returns `NULL`
 *         — logs `FATAL` and aborts if the size computation overflows,
 *         the resulting size is `0`, or the underlying allocator fails.
 */
TEMPLATE_WARN_UNUSED_RESULT void *TemplateAllocMany(usize count, usize elemSize);

/**
 * @brief Resizes an existing allocation to `newSize` bytes, reporting
 *        failure via `errorBuffer` instead of aborting.
 *
 * @param[in,out] ptr Pointer to the pointer to resize. On success, `*ptr`
 *                    is updated to the (possibly moved) new block. On
 *                    failure, `*ptr` is freed and set to `NULL` — the
 *                    original block is never leaked, and the caller is
 *                    never left holding a dangling or stale pointer.
 * @param newSize New size, in bytes. Must be nonzero.
 * @param errorBuffer Error buffer to report failure into.
 * @return `true` on success, `false` if `newSize == 0` or the underlying
 *         reallocation failed (in which case `*ptr` has already been
 *         freed and set to `NULL` before returning).
 *
 * @note Unlike a bare `realloc`, this function never leaves the caller
 *       with two live copies of the pointer to reason about (the
 *       classic `ptr = realloc(ptr, n)` footgun, where a failed
 *       reallocation overwrites the only reference to the still-valid
 *       original block, leaking it). Here, `*ptr` is only ever the
 *       single, currently-correct pointer to hold — either the new
 *       block on success, or `NULL` on failure.
 */
TEMPLATE_WARN_UNUSED_RESULT bool TemplateTryRealloc(
    void **ptr,
    usize newSize,
    char errorBuffer[restrict TEMPLATE_ERROR_BUFFER_SIZE]);

/**
 * @brief Resizes an existing allocation to `newSize` bytes, aborting on
 *        failure.
 *
 * @param[in,out] ptr Pointer to the pointer to resize. Updated to the
 *                    (possibly moved) new block on success.
 * @param newSize New size, in bytes. Must be nonzero.
 *
 * Never returns with `*ptr` unmodified on failure — logs `FATAL` and
 * aborts the process instead (so unlike @ref TemplateTryRealloc, there
 * is no `NULL`-on-failure case to check for here; the process does not
 * survive a failed call).
 */
void TemplateRealloc(void **ptr, usize newSize);

/**
 * @brief Frees `*ptr` and sets `*ptr` to `NULL`.
 *
 * @param[in,out] ptr Pointer to the pointer to free. If `ptr` is `NULL`
 *                    or `*ptr` is already `NULL`, this is a no-op
 *                    (matching `free(NULL)`'s well-defined no-op
 *                    behavior).
 *
 * There is only one variant of this function — freeing cannot fail, so
 * there is no `TemplateTryFree`/`TemplateFree` split the way allocation
 * and reallocation have. Setting `*ptr` to `NULL` after freeing helps
 * guard against accidental use-after-free or double-free at the call
 * site (a second `TemplateFree(&ptr)` on an already-freed pointer is a
 * safe no-op rather than a double free).
 */
void TemplateFree(void **ptr);

#endif