//
// Created by Pedro Tacla Yamada on 5/7/20.
//

#include <spdlog/spdlog.h>

#include "AggregationWorker.h"

namespace filesaver::services
{

AggregationWorker::AggregationWorker (FileSizeService* fileSizeService,
                                      std::shared_ptr<data::WorkQueue<std::shared_ptr<FileEntry>>> queue)
    : BackgroundQueueWorker<std::shared_ptr<FileEntry>> ("AggregationWorker", queue),
      m_fileSizeService (fileSizeService)
{
    spdlog::info ("Starting aggregation thread");
}

void AggregationWorker::handler (std::vector<std::shared_ptr<FileEntry>> entries)
{
    if (m_fileSizeService == nullptr)
    {
        return;
    }

    m_fileSizeService->onFileEntryBulk (entries);
}

} // namespace filesaver::services
