#pragma once

#include <types.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

int vsnprintf(char *buffer, size_t n, const char *fmt, va_list args);
int vsprintf(char *buffer, const char *fmt, va_list args);
int snprintf(char * buffer, size_t n, const char * fmt, ...);
int sprintf(char * buffer, const char * fmt, ...);
int vprintf(const char *fmt, va_list args);
int printf(const char * fmt, ...);
int puts(const char *s);

#ifdef __cplusplus
}
#endif
