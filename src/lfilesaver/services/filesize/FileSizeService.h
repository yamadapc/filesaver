//
// Created by Pedro Tacla Yamada on 28/6/20.
//

#pragma once

#include <boost/filesystem/path.hpp>
#include <memory>
#include <optional>
#include <unordered_map>
#include <unordered_set>

#include "../../data/WorkQueue.h"
#include "../../models/FileEntry.h"
#include "../storage/LevelDbStorageService.h"
#include "../storage/StorageService.h"

namespace filesaver::services
{

class InMemoryFileEntryStore
{
public:
    class Delegate
    {
    public:
        virtual ~Delegate ()
        {
        }

        virtual void onPathFinished (std::shared_ptr<FileEntry> fileEntry) = 0;
    };

    explicit InMemoryFileEntryStore (Delegate& storeDelegate);

    /// Push an entry onto the in-memory store
    void addEntry (std::shared_ptr<FileEntry> entry);

    /// Clean entry from memory
    void cleanEntry (const std::string& filepath);

    /// Get the current in-memory size for a path
    std::optional<off_t> getCurrentSizeAt (const std::string& filepath);

    /// Return true if this path has been finished
    bool isPathFinished (const std::string& filepath);

private:
    /// When an entry is pushed, update its finished state and all of its parents
    void updatePendingAndFinishedState (const boost::filesystem::path& filepath);

    /// When an entry is pushed, update its size and all of its parents
    void updateSizes (const std::shared_ptr<filesaver::FileEntry>& entry);

    /// Adds a size delta recursively to this path and its parents
    void addSizeRecursive (const boost::filesystem::path& filepath, off_t sizeDiff);

    /// Adds a size delta to this path and its parents
    void addSize (const boost::filesystem::path& path, off_t sizeDiff);

    std::unordered_map<std::string, std::shared_ptr<FileEntry>> m_allEntries;
    std::unordered_map<std::string, off_t> m_totalSizes;
    std::unordered_map<std::string, unsigned long> m_pendingChildren;

    Delegate& m_delegate;
};

class FileSizeService : public virtual InMemoryFileEntryStore::Delegate
{
public:
    FileSizeService ();
    ~FileSizeService();

    void onFileEntry (std::shared_ptr<FileEntry> fileEntry);

    off_t getCurrentSizeAt (const boost::filesystem::path& filepath);
    off_t getCurrentSizeAt (const std::string& filepath);

    bool isPathFinished (const boost::filesystem::path& filepath);
    bool isPathFinished (const std::string& filepath);

    void onPathFinished (std::shared_ptr<FileEntry> fileEntry) override;

private:
    void entryWriter ();

    bool running;
    std::shared_ptr<StorageService> m_storageService;
    std::thread storageThread;
    data::WorkQueue<FileSizePair> storageQueue;

    std::mutex m_inMemoryStoreMutex;
    InMemoryFileEntryStore m_inMemoryStore;
    std::unordered_set<std::string> m_finishedPaths;
    std::vector<std::shared_ptr<FileEntry>> m_pathsToClean;
};

} // namespace filesaver::services
