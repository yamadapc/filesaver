//
// Created by Pedro Tacla Yamada on 21/5/20.
//


#pragma once

#include <chrono>
#include <optional>

namespace filesaver
{

/// Simple timer class
/**
 * Provides methods to have an interruptable timer
 */
class SimpleTimer
{
public:
    SimpleTimer () = default;

    /**
     * Start the timer
     */
    void start ();

    /**
     * Stop the timer
     */
    void stop ();

    /**
     * Get the elapsed time from current and past runs
     */
    long long int getElapsedMilliseconds () const;

private:
    std::optional<std::chrono::time_point<std::chrono::steady_clock>> startTime = {};
    long long int totalElapsed = 0;
};

} // namespace filesaver
