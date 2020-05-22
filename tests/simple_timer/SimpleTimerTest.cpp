//
// Created by Pedro Tacla Yamada on 23/5/20.
//

#include <catch2/catch.hpp>
#include <chrono>
#include <thread>

#include <lfilesaver/simple_timer/SimpleTimer.h>

TEST_CASE ("SimpleTimer")
{
    using filesaver::SimpleTimer;

    SimpleTimer simpleTimer;

    SECTION ("Will start with 0 time elapsed")
    {
        REQUIRE (simpleTimer.getElapsedMilliseconds () == 0);
    }

    SECTION ("After starting the timer, we'll get elapsed reported")
    {
        REQUIRE (simpleTimer.getElapsedMilliseconds () == 0);

        simpleTimer.start ();
        std::this_thread::sleep_for (std::chrono::milliseconds (10));
        REQUIRE (simpleTimer.getElapsedMilliseconds () >= 10);
    }

    SECTION ("Stopped portions of the timer won't be reported")
    {
        REQUIRE (simpleTimer.getElapsedMilliseconds () == 0);

        simpleTimer.start ();
        std::this_thread::sleep_for (std::chrono::milliseconds (10));
        simpleTimer.stop ();
        std::this_thread::sleep_for (std::chrono::milliseconds (300));
        REQUIRE (simpleTimer.getElapsedMilliseconds () >= 10);
        REQUIRE (simpleTimer.getElapsedMilliseconds () < 100);
    }

    SECTION ("We can restart the timer")
    {
        REQUIRE (simpleTimer.getElapsedMilliseconds () == 0);

        simpleTimer.start ();
        std::this_thread::sleep_for (std::chrono::milliseconds (10));
        simpleTimer.stop ();
        std::this_thread::sleep_for (std::chrono::milliseconds (500));
        simpleTimer.start ();
        std::this_thread::sleep_for (std::chrono::milliseconds (10));
        simpleTimer.stop ();

        REQUIRE (simpleTimer.getElapsedMilliseconds () >= 20);
        REQUIRE (simpleTimer.getElapsedMilliseconds () < 100);
    }
}
