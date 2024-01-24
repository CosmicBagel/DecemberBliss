#include "log_router.hpp"

#include <stdio.h>

#include <array>

#include "log_gui.hpp"
#include "ray_includes.hpp"

void LogRouter::log_router_enable() {
    SetTraceLogCallback(log_callback_func);
    SetTraceLogLevel(LOG_INFO);
}
void LogRouter::log_router_disable() { SetTraceLogCallback(nullptr); }

// called by raylib, using raylib for log functionality
void LogRouter::log_callback_func(int logType, const char* text, va_list args) {
    const size_t log_type_str_max_len = 10;
    const size_t log_line_buffer_size = 1024;

    std::array<char, log_type_str_max_len> log_type_str = {0};
    switch (logType) {
        case LOG_TRACE:
            strcpy_s(log_type_str.data(), log_type_str_max_len, "TRACE: ");
            break;
        case LOG_DEBUG:
            strcpy_s(log_type_str.data(), log_type_str_max_len, "DEBUG: ");
            break;
        case LOG_INFO:
            strcpy_s(log_type_str.data(), log_type_str_max_len, "INFO: ");
            break;
        case LOG_WARNING:
            strcpy_s(log_type_str.data(), log_type_str_max_len, "WARNING: ");
            break;
        case LOG_ERROR:
            strcpy_s(log_type_str.data(), log_type_str_max_len, "ERROR: ");
            break;
        case LOG_FATAL:
            strcpy_s(log_type_str.data(), log_type_str_max_len, "FATAL: ");
            break;
        default:
            break;
    }
    LogGui::add_log("%s", log_type_str.data());
    printf("%s", log_type_str.data());

    std::array<char, log_line_buffer_size> log_line_buffer = {0};
    vsprintf_s(log_line_buffer.data(), log_line_buffer_size, text, args);

    LogGui::add_log("%s", log_line_buffer.data());
    LogGui::add_log("\n");
    printf("%s", log_line_buffer.data());
    printf("\n");
}
