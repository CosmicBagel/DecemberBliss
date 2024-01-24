#pragma once

#include <chrono>

#include "../MetricsGui/include/metrics_gui/metrics_gui.h"

class Perf_Timer {
   private:
    // Perf_Timer();
    std::chrono::time_point<std::chrono::steady_clock> start;
    MetricsGuiMetric& metric;

    Perf_Timer(const Perf_Timer&) = delete; //copy ctor
    Perf_Timer(const Perf_Timer&&) = delete; //move ctor
    Perf_Timer& operator=(Perf_Timer) = delete; //copy assignment
    Perf_Timer& operator=(Perf_Timer&&) = delete; //move assignment

   public:
    explicit Perf_Timer(MetricsGuiMetric& metric);
    ~Perf_Timer();
};
