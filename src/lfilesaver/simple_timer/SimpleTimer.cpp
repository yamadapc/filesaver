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
    auto now = std::chrono::steady_clock::now ();
    totalElapsed += std::chrono::duration_cast<std::chrono::milliseconds> (now - startTime.value_or (now)).count ();
    startTime = {};
}

long long int SimpleTimer::getElapsedMilliseconds () const
{
    return totalElapsed;
}

} // namespace filesaver
