//
// Created by Pedro Tacla Yamada on 30/6/20.
//


#include "InMemoryFileEntryStore.h"

namespace filesaver::services
{

InMemoryFileEntryStore::InMemoryFileEntryStore ()
{
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

    if (m_delegate.has_value ())
    {
        m_delegate->onPathFinished (entry);
    }

    if (filepath.has_parent_path ())
    {
        auto parentPath = filepath.parent_path ();
        const auto& parentPathStr = parentPath.string ();
        auto it = m_records.find (parentPathStr);

        if (it != m_records.end ())
        {
            auto& record = it->second;
            record.pendingChildren -= 1;
            if (record.pendingChildren <= 0)
            {
                updatePendingAndFinishedState (parentPath);
            }
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

size_t InMemoryFileEntryStore::getHashMapSize ()
{
    return m_records.size ();
}

} // namespace filesaver::services
