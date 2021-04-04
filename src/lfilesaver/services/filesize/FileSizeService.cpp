//
// Created by Pedro Tacla Yamada on 28/6/20.
//

#include "FileSizeService.h"
#include "../../utils/Utils.h"

#include <utility>

namespace filesaver::services
{

FileSizeService::FileSizeService (StorageWorker* storageWorker,
                                  StorageService* storageService,
                                  InMemoryFileSizeService* inMemoryFileSizeService)
    : m_storageWorker (storageWorker),
      m_storageService (storageService),
      m_inMemoryFileSizeService (inMemoryFileSizeService)
{
    m_inMemoryFileSizeService->setDelegate (this);
}

FileSizeService::~FileSizeService ()
{
    m_inMemoryFileSizeService->clearDelegate ();
}

void FileSizeService::onFileEntryBulk (std::vector<std::shared_ptr<FileEntry>> entries)
{
    m_inMemoryFileSizeService->onFileEntryBulk (entries);

    for (const auto& entry : entries)
    {
        m_totalFiles += 1;
        m_totalKnownFiles += entry->children ().size ();
    }
}

void FileSizeService::onFileEntry (std::shared_ptr<FileEntry> entry)
{
    m_inMemoryFileSizeService->onFileEntry (entry);
    m_totalFiles += 1;
    m_totalKnownFiles += entry->children ().size ();
}

off_t FileSizeService::getCurrentSizeAt (const std::string& filepath)
{
    {
        auto maybeSize = m_inMemoryFileSizeService->getCurrentSizeAt (filepath);
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

void FileSizeService::onPathFinished (InMemoryFileEntryStore::Record& record)
{
    auto fileEntry = record.fileEntry;
    FileSizePair pair{fileEntry->filepath.string (), record.totalSize};

    // Log paths bigger than 100MB
    if (pair.getSize() > 100000000) {
        spdlog::debug("FileSizeService - Path finished {} - {}", pair.getFilename(), utils::prettyPrintBytes(pair.getSize()));
    }

    m_storageWorker->push (pair);
}

unsigned long FileSizeService::getTotalFiles ()
{
    return m_totalFiles;
}

unsigned long FileSizeService::getTotalKnownFiles ()
{
    return m_totalKnownFiles;
}

} // namespace filesaver::services
