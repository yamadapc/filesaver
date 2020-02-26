//
// Created by Pedro Tacla Yamada on 2019-08-21.
//

#ifndef FILE_SAVER_WORKERMANAGER_H
#define FILE_SAVER_WORKERMANAGER_H

#include <thread>
#include <vector>

#include "../models/FileEntry.h"
#include "./Worker.h"

namespace filesaver
{

class WorkerManager
{
public:
    WorkQueue<std::shared_ptr<FileEntry>> resultQueue;

    unsigned long getNumWorkers ();

    void start (unsigned int wantedWorkers);
    void stop ();
    void join ();
    void scan (const std::string& filepath);

    unsigned long getFilesProcessed ();

private:
    WorkQueue<boost::filesystem::path> fileWorkQueue;

    std::vector<std::shared_ptr<Worker>> workers;
    std::vector<std::thread> workerThreads;
};

} // namespace filesaver

#endif // FILE_SAVER_WORKERMANAGER_H
