//
// Created by Pedro Tacla Yamada on 13/7/20.
//

#include <catch2/catch.hpp>

#include <lfilesaver/services/stats/InMemoryMetricsReporter.h>

TEST_CASE ("MetricsReporter can hold counters")
{
    filesaver::services::stats::InMemoryMetricsReporter metricsReporter{};

    metricsReporter.counter ("hello", 10);
    metricsReporter.counter ("hello", 10);

    REQUIRE (metricsReporter.getCount ("hello") == 20L);
}

TEST_CASE ("MetricsReporter returns none if no metric")
{
    filesaver::services::stats::InMemoryMetricsReporter metricsReporter;

    auto info = metricsReporter.getTiming ("hello");

    REQUIRE (!info.has_value ());
}

TEST_CASE ("MetricsReporter can track timers")
{
    filesaver::services::stats::InMemoryMetricsReporter metricsReporter;

    metricsReporter.timing ("hello", 10);
    metricsReporter.timing ("hello", 10);
    metricsReporter.timing ("hello", 10);

    auto info = metricsReporter.getTiming ("hello");

    REQUIRE (info.has_value ());
    REQUIRE (info.value ().count == 3);
    REQUIRE (info.value ().average == 10.);
    REQUIRE (info.value ().p50 == 10.);
}

TEST_CASE ("MetricsReporter can track gauges")
{
    filesaver::services::stats::InMemoryMetricsReporter metricsReporter;

    metricsReporter.gauge ("hello", 10);
    metricsReporter.gauge ("hello", 20);

    auto result = metricsReporter.getGauge ("hello");

    REQUIRE (result == 20);
}

TEST_CASE ("MetricsReporter can track timers with `time`")
{
    using namespace filesaver::services::stats;

    InMemoryMetricsReporter metricsReporter;

    long result = timeOperation<long> (metricsReporter,
                                       "hello",
                                       [] () -> long
                                       {
                                           sleep (1);
                                           return 10L;
                                       });

    REQUIRE (result == 10L);

    auto info = metricsReporter.getTiming ("hello");
    REQUIRE (info.has_value ());
    REQUIRE (info.value ().count == 1);
    REQUIRE (info.value ().average >= 1000.);
}
