//
// Created by Pedro Tacla Yamada on 28/6/20.
//

#include "FileSizeService.h"

#include <utility>

namespace filesaver::services
{

FileSizeService::FileSizeService ()
    : m_storageService (std::make_shared<LevelDbStorageService> ("default.db")),
      m_storageQueue (std::make_shared<data::WorkQueue<FileSizePair>> ()),
      m_storageWorker ([&](auto entries) { insertEntries (entries); }, m_storageQueue),
      m_inMemoryStore (this)
{
}

void FileSizeService::onFileEntry (std::shared_ptr<FileEntry> entry)
{
    std::unique_lock<std::mutex> lock{m_inMemoryStoreMutex};
    m_inMemoryStore.addEntry (std::move (entry));
}

off_t FileSizeService::getCurrentSizeAt (const std::string& filepath)
{
    {
        std::unique_lock<std::mutex> lock{m_inMemoryStoreMutex};
        auto maybeSize = m_inMemoryStore.getCurrentSizeAt (filepath);
        if (maybeSize.has_value ())
        {
            return maybeSize.value ();
        }
    }

    auto maybePair = m_storageService->fetchEntry (filepath);
    if (maybePair.has_value ())
    {
        return maybePair.value ().getSize ();
    }

    return 0L;
}

bool FileSizeService::isPathFinished (const boost::filesystem::path& filepath)
{
    return isPathFinished (filepath.string ());
}

bool FileSizeService::isPathFinished (const std::string& filepath)
{
    return m_storageService->fetchEntry (filepath).has_value ();
}

void FileSizeService::onPathFinished (std::shared_ptr<FileEntry> fileEntry)
{
    FileSizePair pair{fileEntry->filepath.string (),
                      m_inMemoryStore.getCurrentSizeAt (fileEntry->filepath.string ()).value_or (0L)};
    m_storageQueue->push (pair);
}

void FileSizeService::insertEntries (std::vector<FileSizePair>& pairs)
{
    for (auto& pair : pairs)
    {
        m_storageService->insertEntry (pair);
    }

    {
        std::unique_lock<std::mutex> lock{m_inMemoryStoreMutex};
        for (auto& pair : pairs)
        {
            m_inMemoryStore.cleanEntry (pair.getFilename ());
        }
    }
}

} // namespace filesaver::services
