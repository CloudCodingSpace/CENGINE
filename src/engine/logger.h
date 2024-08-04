#pragma once 

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

typedef enum {
    LOG_SEVERITY_TRACE,
    LOG_SEVERITY_INFO,
    LOG_SEVERITY_DEBUG,
    LOG_SEVERITY_WARN,
    LOG_SEVERITY_ERROR,
    LOG_SEVERITY_FATAL
} log_severity;

void log_output(log_severity severity, const char* msg, ...);

#define TRACE(msg, ...) log_output(LOG_SEVERITY_TRACE, msg, ##__VA_ARGS__);
#define INFO(msg, ...) log_output(LOG_SEVERITY_INFO, msg, ##__VA_ARGS__);
#define DEBUG(msg, ...) log_output(LOG_SEVERITY_DEBUG, msg, ##__VA_ARGS__);
#define WARN(msg, ...) log_output(LOG_SEVERITY_WARN, msg, ##__VA_ARGS__);
#define ERROR(msg, ...) log_output(LOG_SEVERITY_ERROR, msg, ##__VA_ARGS__);
#define FATAL(msg, ...) log_output(LOG_SEVERITY_FATAL, msg, ##__VA_ARGS__);