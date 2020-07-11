//
// Created by Pedro Tacla Yamada on 4/7/20.
//

#include <spdlog/spdlog.h>

#include "InMemoryFileSizeService.h"

namespace filesaver::services
{

InMemoryFileSizeService::InMemoryFileSizeService ()
{
}

std::optional<off_t> InMemoryFileSizeService::getCurrentSizeAt (const std::string& filepath)
{
    std::unique_lock<std::mutex> lock{m_inMemoryStoreMutex};
    return m_inMemoryStore.getCurrentSizeAt (filepath);
}

void InMemoryFileSizeService::onFileEntry (std::shared_ptr<FileEntry> entry)
{
    std::unique_lock<std::mutex> lock{m_inMemoryStoreMutex};
    m_inMemoryStore.addEntry (std::move (entry));
}

void InMemoryFileSizeService::onFileEntryBulk (std::vector<std::shared_ptr<FileEntry>> entries)
{
    std::unique_lock<std::mutex> lock{m_inMemoryStoreMutex};
    for (auto entry : entries)
    {
        m_inMemoryStore.addEntry (std::move (entry));
    }
}

void InMemoryFileSizeService::cleanEntry (std::string key)
{
    std::unique_lock<std::mutex> lock{m_inMemoryStoreMutex};
    m_inMemoryStore.cleanEntry (key);

    if (rand () % 100000 < 2)
    {
        spdlog::debug ("Current inMemorySize={}", m_inMemoryStore.getHashMapSize ());
    }
}

void InMemoryFileSizeService::cleanEntryBulk (std::vector<std::string> keys)
{
    std::unique_lock<std::mutex> lock{m_inMemoryStoreMutex};
    for (auto key : keys)
    {
        m_inMemoryStore.cleanEntry (key);
    }
}

void InMemoryFileSizeService::setDelegate (InMemoryFileEntryStore::Delegate* delegate)
{
    std::unique_lock<std::mutex> lock{m_inMemoryStoreMutex};
    m_inMemoryStore.setDelegate (delegate);
}

void InMemoryFileSizeService::clearDelegate ()
{
    std::unique_lock<std::mutex> lock{m_inMemoryStoreMutex};
    m_inMemoryStore.clearDelegate ();
}

} // namespace filesaver::services
