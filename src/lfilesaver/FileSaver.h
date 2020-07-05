//
// Created by Pedro Tacla Yamada on 2019-08-20.
//

#pragma once

#include <array>
#include <atomic>
#include <boost/filesystem/path.hpp>
#include <chrono>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>
#include <thread>
#include <unordered_map>

#include "models/FileEntry.h"
#include "server/Server.h"
#include "services/filesize/AggregationWorker.h"
#include "services/filesize/FileSizeService.h"
#include "services/storage/LevelDbStorageService.h"
#include "services/storage/StorageWorker.h"
#include "simple_timer/SimpleTimer.h"
#include "utils/Utils.h"
#include "workers/WorkerManager.h"

namespace filesaver
{

class FileSaver
{
public:
    INJECT (FileSaver (services::FileSizeService*, services::StorageWorker*));
    ~FileSaver ();

    void start ();
    void stop ();
    void scan (const std::string& filepath);

    off_t getCurrentSizeAt (const std::string& filepath);
    bool isPathFinished (boost::filesystem::path& filepath);
    bool areAllTargetsFinished ();
    void setNumWorkers (unsigned int numWorkers);

    std::vector<boost::filesystem::path> getTargets ();

    unsigned long getNumWorkers ();
    unsigned long getTotalFiles ();
    unsigned long getTotalKnownFiles ();
    double getFilesPerSecond ();
    long long int getElapsed ();

    static std::string getVersion ();

private:
    std::vector<boost::filesystem::path> targets;

    std::atomic<unsigned int> numWorkers = 0;
    std::atomic<bool> running = false;
    SimpleTimer timer;
    WorkerManager manager;

    services::FileSizeService* m_fileSizeService;
    services::StorageWorker* m_storageWorker;
    std::shared_ptr<server::Server> server;
    std::thread serverThread;

    services::AggregationWorker m_aggregationWorker;
};

} // namespace filesaver
