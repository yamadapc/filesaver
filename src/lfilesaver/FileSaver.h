//
// Created by Pedro Tacla Yamada on 2019-08-20.
//

#pragma once

#include <atomic>
#include <boost/filesystem/path.hpp>
#include <iostream>
#include <string>

#include "services/filesize/AggregationWorker.h"
#include "services/filesize/FileSizeService.h"
#include "services/storage/LevelDbStorageService.h"
#include "services/storage/StorageWorker.h"
#include "simple_timer/SimpleTimer.h"
#include "workers/WorkerManager.h"

namespace filesaver
{

class FileSaver
{
public:
    INJECT (FileSaver (services::FileSizeService*, services::StorageWorker* /*, server::Server* */));
    ~FileSaver ();

    void start ();
    void stop ();
    static void join ();
    void scan (const std::string& filepath);

    off_t getCurrentSizeAt (const std::string& filepath) const;
    bool isPathFinished (const boost::filesystem::path& filepath) const;
    bool areAllTargetsFinished () const;
    void setNumWorkers (unsigned int numWorkers);

    std::vector<boost::filesystem::path> getTargets ();

    unsigned long getNumWorkers () const;
    unsigned long getTotalFiles () const;
    unsigned long getTotalKnownFiles () const;
    double getFilesPerSecond () const;
    long long int getElapsed () const;

    static std::string getVersion ();

private:
    std::vector<boost::filesystem::path> targets;

    std::atomic<unsigned int> numWorkers = 0;
    std::atomic<bool> running = false;
    SimpleTimer timer;
    WorkerManager manager;

    services::FileSizeService* m_fileSizeService;
    services::StorageWorker* m_storageWorker;
    // server::Server* m_server;
    // std::thread serverThread;

    services::AggregationWorker m_aggregationWorker;
};

} // namespace filesaver
