//
// Created by Pedro Tacla Yamada on 4/4/21.
//

#ifndef FILESAVER_FILECATEGORYWORKER_H
#define FILESAVER_FILECATEGORYWORKER_H

#include <fruit/fruit.h>

#include "../../models/FileSizePair.h"
#include "../BackgroundQueueWorker.h"
#include "./FileCategoryService.h"
#include "./LevelDbFileCategoryStore.h"

namespace filesaver::services
{

class FileCategoryWorker : public BackgroundQueueWorker<FileSizePair>
{
public:
    INJECT (FileCategoryWorker (FileCategoryService* fileCategoryService, FileCategoryStore* fileCategoryStore));
    ~FileCategoryWorker () override;

    void handler (std::vector<FileSizePair> vector) override;

private:
    void handleCategory (const std::shared_ptr<FileCategory>& category,
                         const std::vector<FileSizePair>& fileSizes) const;
    void handleFileSize (const std::shared_ptr<FileCategory>& category, const FileSizePair& fileSize) const;

    FileCategoryService* m_fileCategoryService;
    FileCategoryStore* m_fileCategoryStore;
};

} // namespace filesaver::services

#endif // FILESAVER_FILECATEGORYWORKER_H
