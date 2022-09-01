#pragma once

#include <chrono>

#include "../MetricsGui/include/metrics_gui/metrics_gui.h"

class Perf_Timer {
   private:
    // Perf_Timer();
    std::chrono::time_point<std::chrono::steady_clock> start;
    MetricsGuiMetric& metric;

   public:
    Perf_Timer(MetricsGuiMetric& metric);
    ~Perf_Timer();
};
