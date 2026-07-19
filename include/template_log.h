#ifndef TEMPLATE_LOG_H_
#define TEMPLATE_LOG_H_

#include "template_attributes.h"

#include "template_types.h"

#ifndef VERBOSITY
/*
 * 1 = FATAL
 *
 * 2 = FATAL + ERROR
 *
 * 3 = FATAL + ERROR + WARNING
 *
 * 4 = FATAL + ERROR + WARNING + INFO
 *
 * 5 = FATAL + ERROR + WARNING + INFO + DEBUG (default)
 *
 * 6 = FATAL + ERROR + WARNING + INFO + DEBUG + TRACE
 */
#define VERBOSITY 5
#endif

void TemplateLogInit(void);

#if VERBOSITY >= 1
void TemplateLogFatal(const char *file, i32 line, const char *func, const char *fmt, ...) TEMPLATE_PRINTF(4, 5);
#define LOG_FATAL(...) TemplateLogFatal(__FILE__, __LINE__, __func__, __VA_ARGS__)
#else
#define LOG_FATAL(...) ((void)0)
#endif

#if VERBOSITY >= 2
void TemplateLogError(const char *file, i32 line, const char *func, const char *fmt, ...) TEMPLATE_PRINTF(4, 5);
#define LOG_ERROR(...) TemplateLogError(__FILE__, __LINE__, __func__, __VA_ARGS__)
#else
#define LOG_ERROR(...) ((void)0)
#endif

#if VERBOSITY >= 3
void TemplateLogWarning(const char *file, i32 line, const char *func, const char *fmt, ...) TEMPLATE_PRINTF(4, 5);
#define LOG_WARN(...) TemplateLogWarning(__FILE__, __LINE__, __func__, __VA_ARGS__)
#else
#define LOG_WARN(...) ((void)0)
#endif

#if VERBOSITY >= 4
void TemplateLogInfo(const char *fmt, ...) TEMPLATE_PRINTF(1, 2);
#define LOG_INFO(...) TemplateLogInfo(__VA_ARGS__)
#else
#define LOG_INFO(...) ((void)0)
#endif

#if VERBOSITY >= 5
void TemplateLogDebug(const char *file, i32 line, const char *func, const char *fmt, ...) TEMPLATE_PRINTF(4, 5);
#define LOG_DEBUG(...) TemplateLogDebug(__FILE__, __LINE__, __func__, __VA_ARGS__)
#else
#define LOG_DEBUG(...) ((void)0)
#endif

#if VERBOSITY >= 6
void TemplateLogTrace(const char *file, i32 line, const char *func, const char *fmt, ...) TEMPLATE_PRINTF(4, 5);
#define LOG_TRACE(...) TemplateLogTrace(__FILE__, __LINE__, __func__, __VA_ARGS__)
#else
#define LOG_TRACE(...) ((void)0)
#endif

#endif
