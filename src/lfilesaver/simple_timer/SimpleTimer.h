//
// Created by Pedro Tacla Yamada on 21/5/20.
//


#pragma once

#include <chrono>
#include <optional>

namespace filesaver
{

class SimpleTimer
{
public:
    SimpleTimer () = default;

    void start ();
    void stop ();

    long long int getElapsedMilliseconds () const;

private:
    std::optional<std::chrono::time_point<std::chrono::steady_clock>> startTime = {};
    long long int totalElapsed = 0;
};

} // namespace filesaver
