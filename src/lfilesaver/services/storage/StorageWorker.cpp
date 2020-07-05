//
// Created by Pedro Tacla Yamada on 3/7/20.
//

#include "StorageWorker.h"

#include <algorithm>

namespace filesaver::services
{

void StorageWorker::handler (std::vector<FileSizePair> pairs)
{
    if (m_storageService == nullptr || m_fileSizeService == nullptr)
    {
        return;
    }

    for (auto& pair : pairs)
    {
        m_storageService->insertEntry (pair);
    }

    std::vector<std::string> fileNames{pairs.size ()};
    std::transform (
        pairs.begin (), pairs.end (), fileNames.begin (), [](const auto& pair) { return pair.getFilename (); });
    m_fileSizeService->cleanEntryBulk (fileNames);
}

} // namespace filesaver::services
