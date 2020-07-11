//
// Created by Pedro Tacla Yamada on 3/7/20.
//

#include "StorageWorker.h"

#include <chrono>
#include <thread>

namespace filesaver::services
{

void StorageWorker::handler (std::vector<FileSizePair> pairs)
{
    if (m_storageService == nullptr || m_fileSizeService == nullptr)
    {
        return;
    }

    m_storageService->insertEntryBatch (pairs, 0, pairs.size ());
    for (auto& pair : pairs)
    {
        m_fileSizeService->cleanEntry (pair.getFilename ());
    }
}

} // namespace filesaver::services
