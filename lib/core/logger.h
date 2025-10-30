#pragma once

typedef enum log_level {
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_DEBUG = 2,
} log_level;

void logger_log(log_level level, const char* format, ...);

#define LOG_ERROR(...) logger_log(LOG_LEVEL_ERROR, ##__VA_ARGS__)
#define LOG_DEBUG(...) logger_log(LOG_LEVEL_DEBUG, ##__VA_ARGS__)