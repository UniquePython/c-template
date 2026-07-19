#ifndef TEMPLATE_LOG_H_
#define TEMPLATE_LOG_H_

#include "template_attributes.h"

#include "template_types.h"

#ifndef VERBOSITY
/*
 * 1 = FATAL + NOT_IMPLEMENTED
 *
 * 2 = FATAL + NOT_IMPLEMENTED + ERROR
 *
 * 3 = FATAL + NOT_IMPLEMENTED + ERROR + WARNING + DEPRECATED
 *
 * 4 = FATAL + NOT_IMPLEMENTED + ERROR + WARNING + DEPRECATED + INFO
 *
 * 5 = FATAL + NOT_IMPLEMENTED + ERROR + WARNING + DEPRECATED + INFO + DEBUG (default)
 *
 * 6 = FATAL + NOT_IMPLEMENTED + ERROR + WARNING + DEPRECATED + INFO + DEBUG + TRACE
 */
#define VERBOSITY 5
#endif

void TemplateLogInit(void);

#define LOG_ONCE(LEVEL_MACRO, ...)            \
    do                                        \
    {                                         \
        static bool _template_logged = false; \
        if (!_template_logged)                \
        {                                     \
            _template_logged = true;          \
            LEVEL_MACRO(__VA_ARGS__);         \
        }                                     \
    } while (0)

#if VERBOSITY >= 1
TEMPLATE_NORETURN void TemplateLogFatal(const char *file, i32 line, const char *func, const char *fmt, ...) TEMPLATE_PRINTF(4, 5);
#define LOG_FATAL(...) TemplateLogFatal(__FILE__, __LINE__, __func__, __VA_ARGS__)

TEMPLATE_NORETURN void TemplateLogNotImplemented(const char *file, i32 line, const char *func, const char *fmt, ...) TEMPLATE_PRINTF(4, 5);
#define LOG_NOT_IMPLEMENTED(...) TemplateLogNotImplemented(__FILE__, __LINE__, __func__, __VA_ARGS__)
#else
#define LOG_FATAL(...) ((void)0)
#define LOG_NOT_IMPLEMENTED(...) ((void)0)
#endif

#if VERBOSITY >= 2
void TemplateLogError(const char *file, i32 line, const char *func, const char *fmt, ...) TEMPLATE_PRINTF(4, 5);
#define LOG_ERROR(...) TemplateLogError(__FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_ERROR_ONCE(...) LOG_ONCE(LOG_ERROR, __VA_ARGS__)
#else
#define LOG_ERROR(...) ((void)0)
#define LOG_ERROR_ONCE(...) ((void)0)
#endif

#if VERBOSITY >= 3
void TemplateLogWarning(const char *file, i32 line, const char *func, const char *fmt, ...) TEMPLATE_PRINTF(4, 5);
#define LOG_WARN(...) TemplateLogWarning(__FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_WARN_ONCE(...) LOG_ONCE(LOG_WARN, __VA_ARGS__)

void TemplateLogDeprecated(const char *file, i32 line, const char *func, const char *fmt, ...) TEMPLATE_PRINTF(4, 5);
#define _LOG_DEPRECATED(...) TemplateLogDeprecated(__FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_DEPRECATED(...) LOG_ONCE(_LOG_DEPRECATED, __VA_ARGS__)
#else
#define LOG_WARN(...) ((void)0)
#define LOG_WARN_ONCE(...) ((void)0)
#define LOG_DEPRECATED(...) ((void)0)
#endif

#if VERBOSITY >= 4
void TemplateLogInfo(const char *fmt, ...) TEMPLATE_PRINTF(1, 2);
#define LOG_INFO(...) TemplateLogInfo(__VA_ARGS__)
#define LOG_INFO_ONCE(...) LOG_ONCE(LOG_INFO, __VA_ARGS__)
#else
#define LOG_INFO(...) ((void)0)
#define LOG_INFO_ONCE(...) ((void)0)
#endif

#if VERBOSITY >= 5
void TemplateLogDebug(const char *file, i32 line, const char *func, const char *fmt, ...) TEMPLATE_PRINTF(4, 5);
#define LOG_DEBUG(...) TemplateLogDebug(__FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_DEBUG_ONCE(...) LOG_ONCE(LOG_DEBUG, __VA_ARGS__)
#else
#define LOG_DEBUG(...) ((void)0)
#define LOG_DEBUG_ONCE(...) ((void)0)
#endif

#if VERBOSITY >= 6
void TemplateLogTrace(const char *file, i32 line, const char *func, const char *fmt, ...) TEMPLATE_PRINTF(4, 5);
#define LOG_TRACE(...) TemplateLogTrace(__FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_TRACE_ONCE(...) LOG_ONCE(LOG_TRACE, __VA_ARGS__)
#else
#define LOG_TRACE(...) ((void)0)
#define LOG_TRACE_ONCE(...) ((void)0)
#endif

#endif
