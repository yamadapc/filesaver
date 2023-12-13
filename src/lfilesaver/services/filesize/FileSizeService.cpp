//
// Created by Pedro Tacla Yamada on 28/6/20.
//

#include "FileSizeService.h"
#include "../../models/FileEntry.h"
#include "../../utils/Utils.h"
#include "../storage/StorageWorker.h"

namespace filesaver
{
namespace services
{

FileSizeService::FileSizeService (StorageWorker* storageWorker,
                                  FileCategoryWorker* fileCategoryWorker,
                                  StorageService* storageService,
                                  InMemoryFileSizeService* inMemoryFileSizeService)
    : m_storageWorker (storageWorker),
      m_fileCategoryWorker (fileCategoryWorker),
      m_storageService (storageService),
      m_inMemoryFileSizeService (inMemoryFileSizeService)
{
    m_inMemoryFileSizeService->setDelegate (this);
}

FileSizeService::~FileSizeService ()
{
    m_inMemoryFileSizeService->clearDelegate ();
}

void FileSizeService::onFileEntryBulk (const std::vector<std::shared_ptr<FileEntry>>& entries)
{
    m_inMemoryFileSizeService->onFileEntryBulk (entries);

    for (const auto& entry : entries)
    {
        m_totalFiles += 1;
        m_totalKnownFiles += entry->children ().size ();
    }
}

void FileSizeService::onFileEntry (const std::shared_ptr<FileEntry>& entry)
{
    m_inMemoryFileSizeService->onFileEntry (entry);
    m_totalFiles += 1;
    m_totalKnownFiles += entry->children ().size ();
}

off_t FileSizeService::getCurrentSizeAt (const std::string& filepath) const
{
    {
        const auto maybeSize = m_inMemoryFileSizeService->getCurrentSizeAt (filepath);
        if (maybeSize.has_value ())
        {
            return maybeSize.value ();
        }
    }

    const auto maybePair = m_storageService->fetchEntry (filepath);
    if (maybePair.has_value ())
    {
        return maybePair.value ().getSize ();
    }

    return 0L;
}

bool FileSizeService::isPathFinished (const boost::filesystem::path& filepath) const
{
    return isPathFinished (filepath.string ());
}

bool FileSizeService::isPathFinished (const std::string& filepath) const
{
    return m_inMemoryFileSizeService->getCurrentSizeAt (filepath) == std::nullopt &&
           m_storageService->fetchEntry (filepath).has_value ();
}

void FileSizeService::onPathFinished (InMemoryFileEntryStore::Record& record)
{
    const auto fileEntry = record.fileEntry;
    const FileSizePair pair{fileEntry->filepath.string (), record.totalSize, fileEntry};

    // Log paths bigger than 100MB
    if (pair.getSize () > 100000000)
    {
        spdlog::debug (
            "FileSizeService - Path finished {} - {}", pair.getFilename (), utils::prettyPrintBytes (pair.getSize ()));
    }

    m_storageWorker->push (pair);
    m_fileCategoryWorker->push (pair);
}

unsigned long FileSizeService::getTotalFiles ()
{
    return m_totalFiles;
}

unsigned long FileSizeService::getTotalKnownFiles ()
{
    return m_totalKnownFiles;
}

} // namespace services
} // namespace filesaver
