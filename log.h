#ifndef _LOG_H
#define _LOG_H

#include <stdio.h>

#define LOG_DEBUG_ENABLED

#define LOG_ERROR 4
#define LOG_WARNING 3
#define LOG_INFO 2
#define LOG_DEBUG 1

#ifdef LOG_DEBUG_ENABLED
    #define Log(level, ...) \
        do { \
            printf(__VA_ARGS__); \
            printf("\n"); \
        } while (0)
#else
    #define Log(...)
#endif

#endif
