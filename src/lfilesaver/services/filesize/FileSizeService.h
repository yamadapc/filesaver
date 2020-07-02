//
// Created by Pedro Tacla Yamada on 28/6/20.
//

#pragma once

#include <boost/filesystem/path.hpp>
#include <memory>
#include <optional>
#include <unordered_map>

#include "../../data/WorkQueue.h"
#include "../../models/FileEntry.h"
#include "../BackgroundWorker.h"
#include "../storage/LevelDbStorageService.h"
#include "../storage/StorageService.h"

#include "./InMemoryFileEntryStore.h"

namespace filesaver::services
{

class FileSizeService : public virtual InMemoryFileEntryStore::Delegate
{
public:
    FileSizeService ();

    void onFileEntry (std::shared_ptr<FileEntry> fileEntry);

    off_t getCurrentSizeAt (const std::string& filepath);

    bool isPathFinished (const boost::filesystem::path& filepath);
    bool isPathFinished (const std::string& filepath);

    void onPathFinished (std::shared_ptr<FileEntry> fileEntry) override;
    void insertEntries (std::vector<FileSizePair>& pairs);

private:
    std::shared_ptr<StorageService> m_storageService;
    std::shared_ptr<data::WorkQueue<FileSizePair>> m_storageQueue;
    BackgroundWorker<FileSizePair> m_storageWorker;

    std::mutex m_inMemoryStoreMutex;
    InMemoryFileEntryStore m_inMemoryStore;
};

} // namespace filesaver::services
