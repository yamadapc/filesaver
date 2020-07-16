//
// Created by Pedro Tacla Yamada on 14/7/20.
//

#include "DefaultStatsProvider.h"


namespace filesaver::server
{

DefaultStatsProvider::DefaultStatsProvider (services::stats::ThroughputTracker* throughputTracker,
                                            services::FileSizeService* fileSizeService)
    : m_throughputTracker (throughputTracker), m_fileSizeService (fileSizeService)
{
}

Stats DefaultStatsProvider::getStats ()
{
    auto totalFiles = m_fileSizeService->getTotalFiles ();
    auto millisecondsElapsed = m_throughputTracker->getElapsedMilliseconds ();
    double filesPerSecond = 1000. * (static_cast<double> (totalFiles) / static_cast<double> (millisecondsElapsed));
    return {filesPerSecond, millisecondsElapsed, totalFiles};
}

} // namespace filesaver::server