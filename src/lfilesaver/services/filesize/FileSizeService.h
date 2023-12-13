//
// Created by Pedro Tacla Yamada on 28/6/20.
//

#pragma once

#include <boost/filesystem/path.hpp>
#include <fruit/fruit.h>
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

#include "../../data/WorkQueue.h"
#include "../../models/FileEntry.h"
#include "../BackgroundQueueWorker.h"
#include "../category/FileCategoryWorker.h"
#include "../storage/StorageService.h"
#include "../storage/StorageWorker.h"
#include "./InMemoryFileSizeService.h"

namespace filesaver::services
{

class FileSizeService final : public virtual InMemoryFileEntryStore::Delegate
{
public:
    INJECT (FileSizeService (StorageWorker* storageWorker,
                             FileCategoryWorker* fileCategoryWorker,
                             StorageService* storageService,
                             InMemoryFileSizeService* inMemoryFileSizeService));
    ~FileSizeService () override;

    void onFileEntryBulk (const std::vector<std::shared_ptr<FileEntry>>& entries);
    void onFileEntry (const std::shared_ptr<FileEntry>& fileEntry);
    void onPathFinished (InMemoryFileEntryStore::Record& record) override;

    off_t getCurrentSizeAt (const std::string& filepath) const;
    bool isPathFinished (const boost::filesystem::path& filepath) const;
    bool isPathFinished (const std::string& filepath) const;
    unsigned long getTotalFiles ();
    unsigned long getTotalKnownFiles ();

private:
    std::atomic<unsigned long> m_totalFiles = 0;
    std::atomic<unsigned long> m_totalKnownFiles = 0;

    StorageWorker* m_storageWorker;
    FileCategoryWorker* m_fileCategoryWorker;
    StorageService* m_storageService;
    InMemoryFileSizeService* m_inMemoryFileSizeService;
};

} // namespace filesaver::services
