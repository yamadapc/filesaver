//
// Created by Pedro Tacla Yamada on 2019-08-20.
//

#include "services/filesize/FileSizeService.h"
#include "services/storage/StorageWorker.h"
#include <boost/filesystem/operations.hpp>
#include <thread>
#include <vector>

#include "FileSaver.h"

namespace filesaver
{

FileSaver::FileSaver (services::FileSizeService* fileSizeService, services::StorageWorker* storageWorker
                      /*, server::Server* server */)
    : m_fileSizeService (fileSizeService),
      m_storageWorker (storageWorker),
      // m_server (server),
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

    m_aggregationWorker.start ();
    m_storageWorker->start ();
    // serverThread = std::thread (&server::Server::start, m_server);
}

void FileSaver::join ()
{
    // serverThread.join ();
}

void FileSaver::stop ()
{
    if (!running)
    {
        return;
    }

    m_storageWorker->stop ();
    // m_server->stop ();
    timer.stop ();
    manager.stop ();
    running = false;

    // serverThread.join ();
}

void FileSaver::scan (const std::string& filepath)
{
    const auto target = boost::filesystem::canonical (filepath != "/" ? filepath : "/");
    spdlog::info ("Scanning {}", target.string ());

    if (std::find (targets.begin (), targets.end (), target) == targets.end ())
    {
        manager.scan (target.string ());
        targets.push_back (target);
    }
}

off_t FileSaver::getCurrentSizeAt (const std::string& filepath) const
{
    return m_fileSizeService->getCurrentSizeAt (filepath);
}

bool FileSaver::isPathFinished (const boost::filesystem::path& filepath) const
{
    return m_fileSizeService->isPathFinished (filepath);
}

bool FileSaver::areAllTargetsFinished () const
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

unsigned long FileSaver::getTotalFiles () const
{
    return m_fileSizeService->getTotalFiles ();
}

unsigned long FileSaver::getTotalKnownFiles () const
{
    return m_fileSizeService->getTotalKnownFiles ();
}

double FileSaver::getFilesPerSecond () const
{
    const auto timeDiff = getElapsed ();
    return static_cast<double> (getTotalFiles ()) / (static_cast<double> (timeDiff) / 1000.0);
}

unsigned long FileSaver::getNumWorkers () const
{
    return manager.getNumWorkers ();
}

long long int FileSaver::getElapsed () const
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
    if (result.empty ())
    {
        return "unknown";
    }
    return result;
#else
    return "unknown";
#endif
}

} // namespace filesaver
