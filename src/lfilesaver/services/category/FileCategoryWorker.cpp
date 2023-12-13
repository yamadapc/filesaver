//
// Created by Pedro Tacla Yamada on 4/4/21.
//

#include "FileCategoryWorker.h"
#include "../../utils/Utils.h"

namespace filesaver
{
namespace services
{

FileCategoryWorker::FileCategoryWorker (FileCategoryService* fileCategoryService, FileCategoryStore* fileCategoryStore)
    : m_fileCategoryService (fileCategoryService), m_fileCategoryStore (fileCategoryStore)
{
    start ();
}

FileCategoryWorker::~FileCategoryWorker ()
{
    stop ();

    const auto categories = m_fileCategoryService->getCategories ();
    for (const auto& category : categories)
    {
        const auto sz = category->getSize ();
        spdlog::info ("Category size tag={} size={}", category->getTag (), utils::prettyPrintBytes (sz));
    }
}

void FileCategoryWorker::handler (std::vector<FileSizePair> vector)
{
    spdlog::debug ("Running category matching on files");
    const auto categories = m_fileCategoryService->getCategories ();

    for (const auto& category : categories)
    {
        handleCategory (category, vector);
    }
}

void FileCategoryWorker::handleCategory (const std::shared_ptr<FileCategory>& category,
                                         const std::vector<FileSizePair>& fileSizes) const
{
    spdlog::debug ("Matching {}", category->getName ());

    for (const auto& entry : fileSizes)
    {
        handleFileSize (category, entry);
    }
}

void FileCategoryWorker::handleFileSize (const std::shared_ptr<FileCategory>& category,
                                         const FileSizePair& fileSize) const
{
    const boost::filesystem::path path = fileSize.getFilename ();
    const auto filenamePath = path.filename ();
    const auto match = category->getMatcher ().matches (filenamePath);

    if (match.matches)
    {
        spdlog::debug ("Matched {} : {}", category->getName (), fileSize.getFilename ());
        category->addSize (fileSize.getSize ());

        m_fileCategoryStore->insertPath (category->getTag (), fileSize.getFilename ());
    }
}

} // namespace services
} // namespace filesaver
