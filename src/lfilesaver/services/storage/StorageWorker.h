//
// Created by Pedro Tacla Yamada on 3/7/20.
//


#pragma once

#include <fruit/fruit.h>
#include <spdlog/spdlog.h>

#include "../../models/FileSizePair.h"
#include "../BackgroundQueueWorker.h"
#include "../filesize/InMemoryFileSizeService.h"
#include "../storage/StorageService.h"

namespace filesaver::services
{

class StorageWorker final : public BackgroundQueueWorker<FileSizePair>
{
public:
    INJECT (StorageWorker (StorageService* storageService, InMemoryFileSizeService* fileSizeService))
        : BackgroundQueueWorker ("StorageWorker"),
          m_storageService (storageService),
          m_fileSizeService (fileSizeService)
    {
        spdlog::info ("Starting storage thread");
    }

    ~StorageWorker () override
    {
        stop ();
    }

    void handler (std::vector<FileSizePair> vector) override;

private:
    StorageService* m_storageService = nullptr;
    InMemoryFileSizeService* m_fileSizeService = nullptr;
};

} // namespace filesaver::services
