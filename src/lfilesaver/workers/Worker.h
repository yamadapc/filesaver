//
// Created by Pedro Tacla Yamada on 2019-08-21.
//

#ifndef FILE_SAVER_WORKER_H
#define FILE_SAVER_WORKER_H

#include <boost/filesystem/path.hpp>
#include <chrono>
#include <iostream>
#include <string>

#include "../data/WorkQueue.h"
#include "../models/FileEntry.h"

namespace filesaver
{

class Worker
{
public:
    Worker (data::WorkQueue<boost::filesystem::path>& queue, data::WorkQueue<std::shared_ptr<FileEntry>>& rqueue);

    void start ();
    void stop ();
    void processEntry (boost::filesystem::path& file);
    unsigned long getFilesProcessed ();

private:
    bool running = false;
    unsigned long filesProcessed = 0;

    data::WorkQueue<std::shared_ptr<filesaver::FileEntry>>& resultQueue;
    data::WorkQueue<boost::filesystem::path>& workQueue;
};

} // namespace filesaver

#endif // FILE_SAVER_WORKER_H
