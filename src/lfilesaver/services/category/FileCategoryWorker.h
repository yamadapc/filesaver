//
// Created by Pedro Tacla Yamada on 4/4/21.
//

#ifndef FILESAVER_FILECATEGORYWORKER_H
#define FILESAVER_FILECATEGORYWORKER_H

#include <fruit/fruit.h>
#include <spdlog/spdlog.h>

#include "../../models/FileSizePair.h"
#include "../BackgroundQueueWorker.h"
#include "./FileCategoryService.h"

namespace filesaver::services
{

class FileCategoryWorker : public BackgroundQueueWorker<FileSizePair>
{
public:
    INJECT (FileCategoryWorker (FileCategoryService* fileCategoryService));
    ~FileCategoryWorker ();

    void handler (std::vector<FileSizePair> vector) override;

private:
    FileCategoryService* m_fileCategoryService;
    void handleCategory (const std::shared_ptr<FileCategory>& category, std::vector<FileSizePair>& fileSizes);
    void handleFileSize (const std::shared_ptr<FileCategory>& category, const FileSizePair& fileSize);
};

} // namespace filesaver::services

#endif // FILESAVER_FILECATEGORYWORKER_H
