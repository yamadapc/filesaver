//
// Created by Pedro Tacla Yamada on 14/7/20.
//


#pragma once

#include <fruit/fruit.h>

#include "../../simple_timer/SimpleTimer.h"

namespace filesaver::services::stats
{

struct ScanTimer;

class ThroughputTracker
{
public:
    INJECT (ThroughputTracker (ANNOTATED (ScanTimer, SimpleTimer*) timer));

    /**
     * Get the elapsed time from current and past runs
     */
    long long int getElapsedMilliseconds () const;

private:
    SimpleTimer* m_timer;
};

} // namespace filesaver::services::stats
