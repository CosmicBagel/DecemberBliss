#include "perf_timer.hpp"

#include <chrono>

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::microseconds;

Perf_Timer::Perf_Timer(MetricsGuiMetric& metric) : metric(metric) {
    start = high_resolution_clock::now();
}

Perf_Timer::~Perf_Timer() {
    microseconds elapsed =
        duration_cast<microseconds>(high_resolution_clock::now() - start);
    metric.AddNewValue(static_cast<float>(elapsed.count()));
}
