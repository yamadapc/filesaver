//
// Created by Pedro Tacla Yamada on 4/4/21.
//

#include "FileCategoryWorker.h"

namespace filesaver::services
{

FileCategoryWorker::FileCategoryWorker (FileCategoryService* fileCategoryService)
    : m_fileCategoryService (fileCategoryService)
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
        spdlog::debug ("Matching {}", category->getName ());
        for (const auto& entry : vector)
        {
            auto match = category->getMatcher ().matches (entry.getFilename ());
            if (match.matches)
            {
                spdlog::info ("Matched {} : {}", category->getName (), entry.getFilename ());
            }
        }
    }
}

} // namespace filesaver::services
