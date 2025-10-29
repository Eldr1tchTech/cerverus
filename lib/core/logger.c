#include "logger.h"

#include <stdio.h>
#include <stdarg.h>

const char* colors[] = {
    "\033[0m",  // Reset
    "\033[31m", // Red
    "\033[35m"  // Magenta
};

const char* tags[] = {
    NULL,           // Spacer
    "[ERROR]: ",
    "[DEBUG]: ",
};

void logger_log(log_level level, const char* format, ...) {
    va_list args;
    va_start(args, format);
    printf("%s%s", colors[level], tags[level]);
    vprintf(format, args);
    printf("%s\n", colors[0]);
    va_end(args);
}