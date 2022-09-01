#include "perf_timer.hpp"

#include <chrono>

Perf_Timer::Perf_Timer(MetricsGuiMetric& metric) : metric(metric) {
    using namespace std::chrono;
    start = high_resolution_clock::now();
}

Perf_Timer::~Perf_Timer() {
    using namespace std::chrono;
    microseconds elapsed =
        duration_cast<microseconds>(high_resolution_clock::now() - start);
    metric.AddNewValue((float)elapsed.count());
}
