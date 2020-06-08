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
    void setupDefaultStorage ();
    void setNumWorkers (unsigned int numWorkers);

    std::vector<boost::filesystem::path> getTargets ();
    unsigned long getTotalFiles ();
    unsigned long getTotalKnownFiles ();
    double getFilesPerSecond ();
    unsigned long getNumWorkers ();
    long long int getElapsed ();
    size_t getStorageQueueSize ();
    size_t getInMemoryEntryCount ();

    static std::string getVersion ();

private:
    void entryReader ();
    void entryWriter ();

    off_t getCurrentSizeAtWithoutLock (const std::string& filepath);
    void updateSizes (const std::shared_ptr<filesaver::FileEntry>& entry);
    void onFileSizeChanged (const boost::filesystem::path& filepath, off_t sizeDiff);
    void onFinished (const boost::filesystem::path& filepath);
    void addSize (const boost::filesystem::path& path, off_t sizeDiff);

    bool hasStorage ();

    data::WorkQueue<std::shared_ptr<FileEntry>> storageQueue;

    // This should lock multi-threaded access to these variables
    std::mutex criticalSection;
    std::unordered_map<std::string, std::shared_ptr<FileEntry>> allEntries;
    std::unordered_map<std::string, off_t> totalSizes;
    std::unordered_map<std::string, unsigned long> pendingChildren;

    std::vector<boost::filesystem::path> targets;

    unsigned int numWorkers = 0;

    unsigned long totalFiles = 0;
    unsigned long totalKnownFiles = 0;

    std::unique_ptr<LevelDbStorageService> storageService;
    bool running = false;
    bool storing = false;
    std::thread storageThread;
    std::thread readerThread;
    WorkerManager manager;
    SimpleTimer timer;
};

} // namespace filesaver
