//
// Created by Pedro Tacla Yamada on 2019-08-20.
//

#pragma once

#include <array>
#include <boost/filesystem/path.hpp>
#include <boost/format.hpp>
#include <chrono>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>
#include <thread>
#include <unordered_map>

#include "models/FileEntry.h"
#include "services/filesize/FileSizeService.h"
#include "services/storage/LevelDbStorageService.h"
#include "simple_timer/SimpleTimer.h"
#include "workers/WorkerManager.h"

namespace filesaver
{

std::string prettyPrintBytes (off_t bytes);

class FileSaver
{
public:
    FileSaver ();
    ~FileSaver ();

    void start ();
    void stop ();
    void scan (const std::string& filepath);

    off_t getCurrentSizeAt (const std::string& filepath);
    bool isPathFinished (boost::filesystem::path& filepath);
    bool areAllTargetsFinished ();
    void setNumWorkers (unsigned int numWorkers);

    std::vector<boost::filesystem::path> getTargets ();
    unsigned long getTotalFiles ();
    unsigned long getTotalKnownFiles ();
    double getFilesPerSecond ();
    unsigned long getNumWorkers ();
    long long int getElapsed ();

    static std::string getVersion ();

private:
    void entryReader ();

    std::vector<boost::filesystem::path> targets;
    unsigned int numWorkers = 0;
    unsigned long totalFiles = 0;
    unsigned long totalKnownFiles = 0;

    services::FileSizeService fileSizeService;
    bool running = false;
    std::thread storageThread;
    std::thread readerThread;
    WorkerManager manager;
    SimpleTimer timer;
};

} // namespace filesaver
