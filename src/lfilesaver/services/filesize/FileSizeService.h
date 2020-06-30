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
#include "../storage/LevelDbStorageService.h"
#include "../storage/StorageService.h"

#include "InMemoryFileEntryStore.h"

namespace filesaver::services
{

class FileSizeService : public virtual InMemoryFileEntryStore::Delegate
{
public:
    FileSizeService ();
    ~FileSizeService ();

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
};

} // namespace filesaver::services
