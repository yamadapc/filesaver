//
// Created by Pedro Tacla Yamada on 14/7/20.
//

#include "ThroughputTracker.h"

namespace filesaver::services::stats
{

ThroughputTracker::ThroughputTracker (SimpleTimer* timer) : m_timer (timer)
{
}

long long int ThroughputTracker::getElapsedMilliseconds () const
{
    return m_timer->getElapsedMilliseconds ();
}

} // namespace filesaver::services::stats