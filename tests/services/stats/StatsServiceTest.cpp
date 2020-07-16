//
// Created by Pedro Tacla Yamada on 13/7/20.
//

#include <catch2/catch.hpp>

#include <lfilesaver/services/stats/StatsManager.h>

TEST_CASE ("StatsManager can hold counters")
{
    filesaver::services::stats::StatsManager statsService;

    statsService.counter ("hello", 10);
    statsService.counter ("hello", 10);

    REQUIRE (statsService.getCount ("hello") == 20L);
}

TEST_CASE ("StatsManager returns none if no metric")
{
    filesaver::services::stats::StatsManager statsService;

    auto info = statsService.getTiming ("hello");

    REQUIRE (!info.has_value ());
}

TEST_CASE ("StatsManager can track timers")
{
    filesaver::services::stats::StatsManager statsService;

    statsService.timing ("hello", 10);
    statsService.timing ("hello", 10);
    statsService.timing ("hello", 10);

    auto info = statsService.getTiming ("hello");

    REQUIRE (info.has_value ());
    REQUIRE (info.value ().count == 3);
    REQUIRE (info.value ().average == 10.);
    REQUIRE (info.value ().p50 == 10.);
}

TEST_CASE ("StatsManager can track gauges")
{
    filesaver::services::stats::StatsManager statsService;

    statsService.gauge ("hello", 10);
    statsService.gauge ("hello", 20);

    auto result = statsService.getGauge ("hello");

    REQUIRE (result == 20);
}