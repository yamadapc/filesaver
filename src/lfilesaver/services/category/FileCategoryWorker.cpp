//
// Created by Pedro Tacla Yamada on 4/4/21.
//

#include "FileCategoryWorker.h"
#include "../../utils/Utils.h"

namespace filesaver::services
{

FileCategoryWorker::FileCategoryWorker (FileCategoryService* fileCategoryService, FileCategoryStore* fileCategoryStore)
    : m_fileCategoryService (fileCategoryService), m_fileCategoryStore (fileCategoryStore)
{
    start ();
}

FileCategoryWorker::~FileCategoryWorker ()
{
    stop ();
}

void FileCategoryWorker::handler (std::vector<FileSizePair> vector)
{
    spdlog::debug ("Running category matching on files");
    auto categories = m_fileCategoryService->getCategories ();

    for (const auto& category : categories)
    {
        handleCategory (category, vector);
    }
}

void FileCategoryWorker::handleCategory (const std::shared_ptr<FileCategory>& category,
                                         std::vector<FileSizePair>& fileSizes)
{
    spdlog::debug ("Matching {}", category->getName ());

    for (const auto& entry : fileSizes)
    {
        handleFileSize (category, entry);
    }
}

void FileCategoryWorker::handleFileSize (const std::shared_ptr<FileCategory>& category, const FileSizePair& fileSize)
{
    boost::filesystem::path path = fileSize.getFilename ();
    auto filenamePath = path.filename ();
    auto match = category->getMatcher ().matches (filenamePath);

    if (match.matches)
    {
        spdlog::debug ("Matched {} : {}", category->getName (), fileSize.getFilename ());
        category->addSize (fileSize.getSize ());

        m_fileCategoryStore->insertPath (category->getTag (), fileSize.getFilename ());
    }
}

} // namespace filesaver::services
