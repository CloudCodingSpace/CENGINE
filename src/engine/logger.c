#include "logger.h"

void log_output(log_severity severity, const char* msg, ...) {
    char* mssage;
    char buff[32000];
    va_list list;

    va_start(list, msg);
    vsprintf(buff, msg, list);
    va_end(list);

    if(severity == LOG_SEVERITY_TRACE) {
        char message[] = "[TRACE] ";
        mssage = message;
    }
    else if(severity == LOG_SEVERITY_INFO) {
        char message[] = "[INFO] ";
        mssage = message;
    }
    else if(severity == LOG_SEVERITY_DEBUG) {
        char message[] = "[DEBUG] ";
        mssage = message;
    }
    else if(severity == LOG_SEVERITY_ERROR) {
        char message[] = "[ERROR] ";
        mssage = message;
    }
    else if(severity == LOG_SEVERITY_FATAL) {
        char message[] = "[FATAL] ";
        mssage = message;
    }

    strcat(mssage, buff);
    printf("%s\n", mssage);

    if(severity == LOG_SEVERITY_FATAL || severity == LOG_SEVERITY_ERROR) 
        exit(EXIT_FAILURE);
}