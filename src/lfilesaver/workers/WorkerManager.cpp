//
// Created by Pedro Tacla Yamada on 2019-08-21.
//

#include "WorkerManager.h"

namespace filesaver
{

WorkerManager::WorkerManager ()
    : resultQueue (std::make_shared<data::WorkQueue<std::shared_ptr<FileEntry>>> ()),
      fileWorkQueue (std::make_shared<data::WorkQueue<boost::filesystem::path>> ())
{
}

unsigned long WorkerManager::getNumWorkers ()
{
    return workerThreads.size ();
}

void WorkerManager::start (unsigned int wantedWorkers)
{
    stop ();

    for (unsigned int i = 0; i < wantedWorkers; i++)
    {
        assert(fileWorkQueue != nullptr);
        assert(resultQueue != nullptr);

        auto worker = std::make_shared<Worker> (fileWorkQueue, resultQueue);
        workers.push_back (worker);
        auto thread = std::thread (&Worker::start, worker.get ());
        workerThreads.push_back (std::move (thread));
    }
}

void WorkerManager::stop ()
{
    for (auto& worker : workers)
    {
        worker->stop ();
    }

    join ();

    workers.clear ();
    workerThreads.clear ();
}

void WorkerManager::join ()
{
    for (auto& workerThread : workerThreads)
    {
        workerThread.join ();
    }
}

void WorkerManager::scan (const std::string& filepath)
{
    fileWorkQueue->push ({filepath});
}

unsigned long WorkerManager::getFilesProcessed ()
{
    unsigned long filesProcessed = 0;
    for (auto& worker : workers)
    {
        filesProcessed += worker->getFilesProcessed ();
    }
    return filesProcessed;
}

std::shared_ptr<data::WorkQueue<std::shared_ptr<FileEntry>>> WorkerManager::getResultQueue ()
{
    return resultQueue;
}

} // namespace filesaver
