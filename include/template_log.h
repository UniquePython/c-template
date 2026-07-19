#ifndef TEMPLATE_LOG_H_
#define TEMPLATE_LOG_H_

#include "template_attributes.h"

#include "template_types.h"

#ifndef VERBOSITY
#define VERBOSITY 4 // 0 = silent, 1 = ERROR, 2 = ERROR+WARNING, 3 = ERROR+WARNING+INFO, 4 = ERROR+WARNING+INFO+DEBUG
#endif

void TemplateLogInit(void);

#if VERBOSITY >= 1
void TemplateLogError(const char *file, i32 line, const char *func, const char *fmt, ...) TEMPLATE_PRINTF(4, 5);
#define LOG_ERROR(...) TemplateLogError(__FILE__, __LINE__, __func__, __VA_ARGS__)
#else
#define LOG_ERROR(...) ((void)0)
#endif

#if VERBOSITY >= 2
void TemplateLogWarning(const char *fmt, ...) TEMPLATE_PRINTF(1, 2);
#define LOG_WARN(...) TemplateLogWarning(__VA_ARGS__)
#else
#define LOG_WARN(...) ((void)0)
#endif

#if VERBOSITY >= 3
void TemplateLogInfo(const char *fmt, ...) TEMPLATE_PRINTF(1, 2);
#define LOG_INFO(...) TemplateLogInfo(__VA_ARGS__)
#else
#define LOG_INFO(...) ((void)0)
#endif

#if VERBOSITY >= 4
void TemplateLogDebug(const char *file, i32 line, const char *func, const char *fmt, ...) TEMPLATE_PRINTF(4, 5);
#define LOG_DEBUG(...) TemplateLogDebug(__FILE__, __LINE__, __func__, __VA_ARGS__)
#else
#define LOG_DEBUG(...) ((void)0)
#endif

#endif
