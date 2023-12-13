//
// Created by Pedro Tacla Yamada on 14/7/20.
//

#include "DefaultStatsProvider.h"


namespace filesaver::server
{

static constexpr double MILLISECONDS_IN_SECOND = 1000.;

DefaultStatsProvider::DefaultStatsProvider (services::stats::ThroughputTracker* throughputTracker,
                                            services::FileSizeService* fileSizeService)
    : m_throughputTracker (throughputTracker), m_fileSizeService (fileSizeService)
{
}

Stats DefaultStatsProvider::getStats ()
{
    const auto totalFiles = m_fileSizeService->getTotalFiles ();
    const auto millisecondsElapsed = m_throughputTracker->getElapsedMilliseconds ();
    const double filesPerSecond =
        MILLISECONDS_IN_SECOND * (static_cast<double> (totalFiles) / static_cast<double> (millisecondsElapsed));
    return {filesPerSecond, millisecondsElapsed, totalFiles};
}

} // namespace filesaver::server