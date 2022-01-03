#include "log_router.h"
#include "ray_includes.h"
#include <stdio.h>

Log_Gui log_gui;

// called by raylib, using raylib for log functionality
void log_callback_func(int logType, const char* text, va_list args);

void log_router_enable() {
    SetTraceLogCallback(log_callback_func);
    SetTraceLogLevel(LOG_INFO);
}

void log_router_disable() {
    SetTraceLogCallback(nullptr);
}

#define LOG_LINE_BUFFER_SIZE 1024
char logLineBuffer[LOG_LINE_BUFFER_SIZE] = {};
void log_callback_func(int logType, const char *text, va_list args)
{
    char logTypeStr[10] = {};
    switch (logType)
    {
    case LOG_TRACE:
        strcpy_s(logTypeStr, 10, "TRACE: ");
        break;
    case LOG_DEBUG:
        strcpy_s(logTypeStr, 10, "DEBUG: ");
        break;
    case LOG_INFO:
        strcpy_s(logTypeStr, 10, "INFO: ");
        break;
    case LOG_WARNING:
        strcpy_s(logTypeStr, 10, "WARNING: ");
        break;
    case LOG_ERROR:
        strcpy_s(logTypeStr, 10, "ERROR: ");
        break;
    case LOG_FATAL:
        strcpy_s(logTypeStr, 10, "FATAL: ");
        break;
    default:
        break;
    }
    log_gui.add_log("%s", logTypeStr);
    printf("%s", logTypeStr);

    vsprintf_s(logLineBuffer, LOG_LINE_BUFFER_SIZE, text, args);

    log_gui.add_log("%s", logLineBuffer);
    log_gui.add_log("\n");
    printf("%s", logLineBuffer);
    printf("\n");
}
