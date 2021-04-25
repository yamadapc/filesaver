//
// Created by Pedro Tacla Yamada on 4/4/21.
//

#include "FileCategoryWorker.h"
#include "../../utils/Utils.h"

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
    auto categories = m_fileCategoryService->getCategories ();

    for (const auto& category : categories)
    {
        auto sz = category->getSize ();
        spdlog::info ("Category {} - Size {}", category->getName (), utils::prettyPrintBytes (sz));
    }
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
            bool matched = false;

            {
                boost::filesystem::path path = entry.getFilename ();
                auto filenamePath = path.filename ();
                auto match = category->getMatcher ().matches (filenamePath);

                if (match.isRecursive && match.matches)
                {
                    m_recursiveCategoryCache[path.string ()] = category;
                }

                if (match.matches)
                {
                    matched = true;
                    spdlog::debug ("Matched {} : {}", category->getName (), entry.getFilename ());
                }
            }

            if (matched)
            {
                category->addSize (entry.getSize ());
            }
        }
    }
}

} // namespace filesaver::services
