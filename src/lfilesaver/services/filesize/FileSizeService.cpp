//
// Created by Pedro Tacla Yamada on 28/6/20.
//

#include "FileSizeService.h"

#include <utility>

namespace filesaver::services
{

FileSizeService::FileSizeService ()
    : m_storageService (std::make_shared<LevelDbStorageService> ("default.db")), m_inMemoryStore (*this)
{
    running = true;
    storageThread = std::thread (&FileSizeService::entryWriter, this);
    m_storageService->createTables ();
}

FileSizeService::~FileSizeService ()
{
    running = false;
    storageThread.join ();
}

void FileSizeService::onFileEntry (std::shared_ptr<FileEntry> entry)
{
    std::unique_lock<std::mutex> lock{m_inMemoryStoreMutex};
    m_inMemoryStore.addEntry (std::move (entry));
}

off_t FileSizeService::getCurrentSizeAt (const boost::filesystem::path& filepath)
{
    return getCurrentSizeAt (filepath.string ());
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
    storageQueue.push (pair);
}

void FileSizeService::entryWriter ()
{
    while (running)
    {
        std::vector<FileSizePair> pairs;
        while (storageQueue.size ())
        {
            auto pair = storageQueue.frontWithTimeout (std::chrono::milliseconds (100));
            if (pair.has_value ())
            {
                pairs.push_back (pair.value ());
            }
            else
            {
                break;
            }
        }

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
}

InMemoryFileEntryStore::InMemoryFileEntryStore (Delegate& storeDelegate) : m_delegate (storeDelegate)
{
}

void InMemoryFileEntryStore::addEntry (std::shared_ptr<FileEntry> entry)
{
    auto filepath = entry->filepath;
    auto filepathStr = entry->filepath.string ();

    m_records[filepathStr] = {entry, 0, 0};
    auto& record = m_records[filepathStr];

    updateSizes (entry);

    if (!entry->isDirectory ())
    {
        record.pendingChildren = 0;
        updatePendingAndFinishedState (entry->filepath);
    }
    else
    {
        record.pendingChildren = entry->children ().size ();
        if (record.pendingChildren == 0)
        {
            updatePendingAndFinishedState (entry->filepath);
        }
    }
}

void InMemoryFileEntryStore::cleanEntry (const std::string& filepath)
{
    m_records.erase (filepath);
}

void InMemoryFileEntryStore::updateSizes (const std::shared_ptr<filesaver::FileEntry>& entry)
{
    auto previousSize = getCurrentSizeAt (entry->filepath.string ()).value_or (0L);
    auto sizeDiff = entry->size - previousSize;
    addSizeRecursive (entry->filepath, sizeDiff);
}

std::optional<off_t> InMemoryFileEntryStore::getCurrentSizeAt (const std::string& filepath)
{
    auto totalSizeIt = m_records.find (filepath);
    if (totalSizeIt == m_records.end ())
    {
        return std::optional<off_t>{};
    }
    return totalSizeIt->second.totalSize;
}

bool InMemoryFileEntryStore::isPathFinished (const std::string& filepath)
{
    auto pendingIt = m_records.find (filepath);
    if (pendingIt == m_records.end ())
    {
        return false;
    }
    return pendingIt->second.pendingChildren <= 0;
}

void InMemoryFileEntryStore::addSizeRecursive (const boost::filesystem::path& filepath, off_t sizeDiff)
{
    addSize (filepath, sizeDiff);

    // This is faster than using .has_parent_path or .parent_path because it'll
    // do less copying/alloc.
    boost::filesystem::path path = {filepath};
    do
    {
        path = path.remove_filename ();
        addSize (path, sizeDiff);
    } while (!path.empty ());
}

void InMemoryFileEntryStore::updatePendingAndFinishedState (const boost::filesystem::path& filepath)
{
    const auto& filepathStr = filepath.string ();
    auto entry = m_records[filepathStr].fileEntry;
    entry->isFinished = true;
    m_delegate.onPathFinished (entry);

    if (filepath.has_parent_path ())
    {
        auto parentPath = filepath.parent_path ();
        const auto& parentPathStr = parentPath.string ();
        auto& record = m_records[parentPathStr];

        record.pendingChildren -= 1;
        if (record.pendingChildren <= 0)
        {
            updatePendingAndFinishedState (parentPath);
        }
    }
}

void InMemoryFileEntryStore::addSize (const boost::filesystem::path& path, off_t sizeDiff)
{
    auto parentIt = m_records.find (path.string ());
    if (parentIt != m_records.end ())
    {
        parentIt->second.totalSize += sizeDiff;
    }
}

} // namespace filesaver::services
