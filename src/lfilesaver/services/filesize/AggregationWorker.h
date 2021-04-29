//
// Created by Pedro Tacla Yamada on 5/7/20.
//


#pragma once

#include <memory>

#include "../../models/FileEntry.h"
#include "../BackgroundQueueWorker.h"

#include "./FileSizeService.h"

namespace filesaver::services
{

class AggregationWorker : public BackgroundQueueWorker<std::shared_ptr<FileEntry>>
{
public:
    AggregationWorker (FileSizeService* fileSizeService, std::shared_ptr<data::WorkQueue<std::shared_ptr<FileEntry>>>);
    ~AggregationWorker () override
    {
        spdlog::info ("Shutting-down AggregationWorker");
        stop ();
    };

    void handler (std::vector<std::shared_ptr<FileEntry>> vector) override;

private:
    FileSizeService* m_fileSizeService = nullptr;
};

} // namespace filesaver::services
