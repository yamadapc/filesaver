//
// Created by Pedro Tacla Yamada on 14/7/20.
//


#pragma once

#include <fruit/fruit.h>

#include "../../services/filesize/FileSizeService.h"
#include "../../services/stats/ThroughputTracker.h"
#include "./StatsProvider.h"

namespace filesaver::server
{

class DefaultStatsProvider : public StatsProvider
{
public:
    INJECT (DefaultStatsProvider (
        services::stats::ThroughputTracker* throughputTracker,
        services::FileSizeService* fileSizeService
    ));

    Stats getStats () override;

private:
    services::stats::ThroughputTracker* m_throughputTracker;
    services::FileSizeService* m_fileSizeService;
};


} // namespace filesaver::server
