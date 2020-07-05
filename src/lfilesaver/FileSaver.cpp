//
// Created by Pedro Tacla Yamada on 2019-08-20.
//

#include "server/Server.h"

#include "FileSaver.h"

namespace filesaver
{

FileSaver::FileSaver (services::FileSizeService* fileSizeService, services::StorageWorker* storageWorker)
    : m_fileSizeService (fileSizeService),
      m_storageWorker (storageWorker),
      server (std::make_shared<server::Server> (
          [&]() {
              return server::Stats{getFilesPerSecond (), getElapsed (), getTotalFiles ()};
          },
          [&](std::string path) { return m_fileSizeService->getCurrentSizeAt (path); })),
      m_aggregationWorker (m_fileSizeService, manager.getResultQueue ())
{
}

FileSaver::~FileSaver ()
{
    stop ();
}

void FileSaver::start ()
{
    if (numWorkers <= 0)
    {
        numWorkers = std::thread::hardware_concurrency () * 2;
    }

    spdlog::info ("FileSaver {} - Starting with {} threads", getVersion (), numWorkers);

    running = true;
    timer.start ();
    manager.start (numWorkers);

    m_aggregationWorker.start();
    m_storageWorker->start ();
    serverThread = std::thread (&server::Server::start, server);
}

void FileSaver::stop ()
{
    if (!running)
    {
        return;
    }

    m_storageWorker->stop ();
    server->stop ();
    timer.stop ();
    manager.stop ();
    running = false;

    serverThread.join ();
}

void FileSaver::scan (const std::string& filepath)
{
    auto target = filepath != "/" ? boost::filesystem::path{filepath}.remove_trailing_separator () : "/";
    if (std::find (targets.begin (), targets.end (), target) == targets.end ())
    {
        manager.scan (target.string ());
        targets.push_back (target);
    }
}

off_t FileSaver::getCurrentSizeAt (const std::string& filepath)
{
    return m_fileSizeService->getCurrentSizeAt (filepath);
}

bool FileSaver::isPathFinished (boost::filesystem::path& filepath)
{
    return m_fileSizeService->isPathFinished (filepath);
}

bool FileSaver::areAllTargetsFinished ()
{
    for (auto& target : targets)
    {
        if (!isPathFinished (target))
        {
            return false;
        }
    }

    return true;
}

std::vector<boost::filesystem::path> FileSaver::getTargets ()
{
    return targets;
}

unsigned long FileSaver::getTotalFiles ()
{
    return m_fileSizeService->getTotalFiles ();
}

unsigned long FileSaver::getTotalKnownFiles ()
{
    return m_fileSizeService->getTotalKnownFiles ();
}

double FileSaver::getFilesPerSecond ()
{
    auto timeDiff = getElapsed ();
    return static_cast<double> (getTotalFiles ()) / (static_cast<double> (timeDiff) / 1000.0);
}

unsigned long FileSaver::getNumWorkers ()
{
    return manager.getNumWorkers ();
}

long long int FileSaver::getElapsed ()
{
    return timer.getElapsedMilliseconds ();
}

void FileSaver::setNumWorkers (unsigned int _numWorkers)
{
    numWorkers = _numWorkers;
}

std::string FileSaver::getVersion ()
{
#ifdef GIT_SHORT_HASH
    std::string result{GIT_SHORT_HASH};
    return result;
#else
    return "unknown";
#endif
}

} // namespace filesaver
