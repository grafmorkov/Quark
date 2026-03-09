#ifndef QUARK_LOGGER_H
#define QUARK_LOGGER_H

#include <stdio.h>
#include <time.h>

#include "quark/colors.h"

#define GET_TIME() ({ \
    clock_t c = clock(); \
    (double)c * 1000 / CLOCKS_PER_SEC; \
})

#define LOG_INFO(txt) do { \
    printf("(%f ms) INFO: %s\n", GET_TIME(), txt); \
} while(0)

#define LOG_WARN(txt) do { \
    SET_COLOR(YELLOW); \
    printf("(%f ms) WARN: %s\n", GET_TIME(), txt); \
    RESET_COLOR(); \
} while(0)

#define LOG_ERROR(txt) do { \
    SET_COLOR(RED); \
    fprintf(stderr, "(%f ms) ERROR: %s\n", GET_TIME(), txt); \
    RESET_COLOR(); \
} while(0)

#define FATAL(txt) do { \
    LOG_ERROR(txt); \
    exit(1); \
} while(0)

#endif