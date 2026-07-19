#ifndef TEMPLATE_ATTRIBUTES_H_
#define TEMPLATE_ATTRIBUTES_H_

// Portable wrappers around GCC/Clang function attributes used across the
// project. Requires GCC or Clang.

#define TEMPLATE_PRINTF(fmtIndex, argIndex) __attribute__((format(printf, fmtIndex, argIndex)))
#define TEMPLATE_CONST __attribute__((const))
#define TEMPLATE_PURE __attribute__((pure))
#define TEMPLATE_WARN_UNUSED_RESULT __attribute__((warn_unused_result))

#define TEMPLATE_DISCARD_CONST(ptr)                                                                                                       \
    (__extension__({                                                                                                                      \
        __typeof__(ptr) TEMPLATE_DISCARD_CONST_tmp_ = (ptr);                                                                              \
        _Pragma("GCC diagnostic push")                                                                                                    \
            _Pragma("GCC diagnostic ignored \"-Wcast-qual\"") void *TEMPLATE_DISCARD_CONST_result_ = (void *)TEMPLATE_DISCARD_CONST_tmp_; \
        _Pragma("GCC diagnostic pop")                                                                                                     \
            TEMPLATE_DISCARD_CONST_result_;                                                                                               \
    }))

#endif