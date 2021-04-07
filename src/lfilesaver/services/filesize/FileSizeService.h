//
// Created by Pedro Tacla Yamada on 28/6/20.
//

#pragma once

#include <atomic>
#include <boost/filesystem/path.hpp>
#include <fruit/fruit.h>
#include <memory>
#include <optional>
#include <unordered_map>

#include "../../data/WorkQueue.h"
#include "../../models/FileEntry.h"
#include "../BackgroundQueueWorker.h"
#include "../storage/StorageService.h"
#include "../storage/StorageWorker.h"
#include "./InMemoryFileSizeService.h"

namespace filesaver::services
{

class FileSizeService : public virtual InMemoryFileEntryStore::Delegate
{
public:
    INJECT (FileSizeService (
        StorageWorker* storageWorker,
        StorageService* storageService,
        InMemoryFileSizeService* inMemoryFileSizeService
    ));
    ~FileSizeService ();

    void onFileEntryBulk (std::vector<std::shared_ptr<FileEntry>> entries);
    void onFileEntry (std::shared_ptr<FileEntry> fileEntry);
    void onPathFinished (InMemoryFileEntryStore::Record& record) override;

    off_t getCurrentSizeAt (const std::string& filepath);
    bool isPathFinished (const boost::filesystem::path& filepath);
    bool isPathFinished (const std::string& filepath);
    unsigned long getTotalFiles ();
    unsigned long getTotalKnownFiles ();

private:
    std::atomic<unsigned long> m_totalFiles = 0;
    std::atomic<unsigned long> m_totalKnownFiles = 0;

    StorageWorker* m_storageWorker;
    StorageService* m_storageService;
    InMemoryFileSizeService* m_inMemoryFileSizeService;
};

} // namespace filesaver::services
