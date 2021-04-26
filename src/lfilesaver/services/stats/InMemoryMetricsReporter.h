//
// Created by Pedro Tacla Yamada on 13/7/20.
//


#pragma once

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/thread/synchronized_value.hpp>
#include <fruit/fruit.h>
#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>

#include "../../simple_timer/SimpleTimer.h"

namespace filesaver::services::stats
{

using namespace boost::accumulators;

using TimerAccumulatorSet =
    accumulator_set<double,
                    stats::stats<tag::mean, tag::count, tag::max, tag::min, tag::median, tag::tail_quantile<right>>>;

struct TimerStat
{
    boost::synchronized_value<TimerAccumulatorSet> accumulator =
        boost::synchronized_value (TimerAccumulatorSet{tag::tail<right>::cache_size = 3000});

    struct Info
    {
        double average;
        unsigned long count;
        double max;
        double min;
        double p25;
        double p50;
        double p75;
        double p90;
        double p99;
    } __attribute__ ((aligned (128)));

    Info getInfo ();
    void push (double value);
} __attribute__ ((aligned (128)));

struct CounterStat
{
    boost::synchronized_value<long long int> count = 0;
} __attribute__ ((aligned (128)));

struct GaugeStat
{
    boost::synchronized_value<long long int> currentValue = 0;
} __attribute__ ((aligned (128)));

class MetricsReporter
{
public:
    virtual ~MetricsReporter () = default;

    virtual void counter (const std::string& name, long long int increment) = 0;
    virtual void gauge (const std::string& name, long long int value) = 0;
    virtual void timing (const std::string& name, double time) = 0;
};

class InMemoryMetricsReporter : public MetricsReporter
{
public:
    INJECT (InMemoryMetricsReporter ()) = default;
    ~InMemoryMetricsReporter () override = default;

    long long int getGauge (const std::string& name);
    long long int getCount (const std::string& name);
    std::optional<TimerStat::Info> getTiming (const std::string& name);

    void counter (const std::string& name, long long int increment) override;
    void gauge (const std::string& name, long long int value) override;
    void timing (const std::string& name, double time) override;

private:
    std::unordered_map<std::string, CounterStat> m_counters;
    std::unordered_map<std::string, GaugeStat> m_gauges;
    std::unordered_map<std::string, TimerStat> m_timers;
};

template <typename T, typename Fn, typename Reporter>
T timeOperation (Reporter& reporter, const std::string& name, Fn fn)
{
    SimpleTimer timer{};
    timer.start ();
    T result = fn ();
    timer.stop ();
    reporter.timing (name, static_cast<double> (timer.getElapsedMilliseconds ()));
    return result;
}

} // namespace filesaver::services::stats
