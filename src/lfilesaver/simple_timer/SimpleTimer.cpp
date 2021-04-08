//
// Created by Pedro Tacla Yamada on 21/5/20.
//

#include "SimpleTimer.h"

namespace filesaver
{

void SimpleTimer::start ()
{
    startTime = std::chrono::steady_clock::now ();
}

void SimpleTimer::stop ()
{
    if (!startTime.has_value ())
    {
        return;
    }

    auto now = std::chrono::steady_clock::now ();
    totalElapsed += std::chrono::duration_cast<std::chrono::milliseconds> (now - startTime.value ()).count ();
    startTime = std::optional<std::chrono::time_point<std::chrono::steady_clock>> ();
}

long long int SimpleTimer::getElapsedMilliseconds () const
{
    if (startTime.has_value ())
    {
        auto now = std::chrono::steady_clock::now ();
        auto currentElapsed = std::chrono::duration_cast<std::chrono::milliseconds> (now - startTime.value ()).count ();
        return totalElapsed + currentElapsed;
    }

    return totalElapsed;
}

} // namespace filesaver
