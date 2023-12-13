//
// Created by Pedro Tacla Yamada on 13/7/20.
//

#include "InMemoryMetricsReporter.h"

namespace filesaver::services::stats
{

TimerStat::Info TimerStat::getInfo ()
{
    auto acc = accumulator.synchronize ();
    const auto _average = mean (*acc);
    const auto _count = count (*acc);
    const auto _max = max (*acc);
    const auto _min = min (*acc);
    const auto _p25 = quantile (*acc, quantile_probability = 0.25);
    const auto _p50 = median (*acc);
    const auto _p75 = quantile (*acc, quantile_probability = 0.75);
    const auto _p90 = quantile (*acc, quantile_probability = 0.90);
    const auto _p99 = quantile (*acc, quantile_probability = 0.99);

    return {_average, _count, _max, _min, _p25, _p50, _p75, _p90, _p99};
}

void TimerStat::push (double value)
{
    auto acc = accumulator.synchronize ();
    (*acc) (value);
}

long long int InMemoryMetricsReporter::getGauge (const std::string& name)
{
    const auto it = m_gauges.find (name);
    return it != m_gauges.end () ? *it->second.currentValue : 0L;
}

long long int InMemoryMetricsReporter::getCount (const std::string& name)
{
    const auto it = m_counters.find (name);
    return it != m_counters.end () ? *it->second.count : 0L;
}

std::optional<TimerStat::Info> InMemoryMetricsReporter::getTiming (const std::string& name)
{
    const auto it = m_timers.find (name);
    return it != m_timers.end () ? std::optional<TimerStat::Info>{it->second.getInfo ()}
                                 : std::nullopt;
}

void InMemoryMetricsReporter::counter (const std::string& name, long long int increment)
{
    const auto maybeCounter = m_counters.find (name);
    auto& counter = maybeCounter == m_counters.end () ? m_counters[name] = CounterStat{} : maybeCounter->second;
    *counter.count += increment;
}

void InMemoryMetricsReporter::gauge (const std::string& name, long long int value)
{
    const auto maybeGauge = m_gauges.find (name);
    auto& gauge = maybeGauge == m_gauges.end () ? m_gauges[name] = GaugeStat{} : maybeGauge->second;
    *gauge.currentValue = value;
}

void InMemoryMetricsReporter::timing (const std::string& name, double time)
{
    const auto maybeTimer = m_timers.find (name);
    auto& timer = maybeTimer == m_timers.end () ? m_timers[name] = TimerStat{} : maybeTimer->second;
    timer.push (time);
}

} // namespace filesaver::services::stats
