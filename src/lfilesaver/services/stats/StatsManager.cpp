//
// Created by Pedro Tacla Yamada on 13/7/20.
//

#include "StatsManager.h"

namespace filesaver::services::stats
{

TimerStat::Info TimerStat::getInfo ()
{
    auto acc = accumulator.synchronize ();
    auto _average = mean (*acc);
    auto _count = count (*acc);
    auto _max = max (*acc);
    auto _min = min (*acc);
    auto _p25 = quantile (*acc, quantile_probability = 0.25);
    auto _p50 = median (*acc);
    auto _p75 = quantile (*acc, quantile_probability = 0.75);
    auto _p90 = quantile (*acc, quantile_probability = 0.90);
    auto _p99 = quantile (*acc, quantile_probability = 0.99);

    return {_average, _count, _max, _min, _p25, _p50, _p75, _p90, _p99};
}

void TimerStat::push (double value)
{
    auto acc = accumulator.synchronize ();
    (*acc) (value);
}

long long int StatsManager::getGauge (const std::string& name)
{
    auto it = m_gauges.find (name);
    return it != m_gauges.end () ? *it->second.currentValue : 0L;
}

long long int StatsManager::getCount (const std::string& name)
{
    auto it = m_counters.find (name);
    return it != m_counters.end () ? *it->second.count : 0L;
}

std::optional<TimerStat::Info> StatsManager::getTiming (const std::string& name)
{
    auto it = m_timers.find (name);
    return it != m_timers.end () ? std::optional<TimerStat::Info>{it->second.getInfo ()}
                                 : std::optional<TimerStat::Info>{};
}

void StatsManager::counter (const std::string& name, long long int increment)
{
    auto maybeCounter = m_counters.find (name);
    auto& counter = maybeCounter == m_counters.end () ? m_counters[name] = CounterStat{} : maybeCounter->second;
    *counter.count += increment;
}

void StatsManager::gauge (const std::string& name, long long int value)
{
    auto maybeGauge = m_gauges.find (name);
    auto& gauge = maybeGauge == m_gauges.end () ? m_gauges[name] = GaugeStat{} : maybeGauge->second;
    *gauge.currentValue = value;
}

void StatsManager::timing (const std::string& name, double time)
{
    auto maybeTimer = m_timers.find (name);
    auto& timer = maybeTimer == m_timers.end () ? m_timers[name] = TimerStat{} : maybeTimer->second;
    timer.push (time);
}

} // namespace filesaver::services::stats
