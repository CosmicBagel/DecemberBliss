#pragma once
#include <stdarg.h>

class LogRouter {
   private:
    // called by raylib, using raylib for log functionality
    static void log_callback_func(int logType, const char* text, va_list args);

   public:
    static void log_router_enable();
    static void log_router_disable();
};
