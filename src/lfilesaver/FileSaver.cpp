//
// Created by Pedro Tacla Yamada on 2019-08-20.
//

#include "FileSaver.h"

namespace filesaver
{

std::string prettyPrintBytes (off_t bytes)
{
    const std::array<std::string, 7> suffixes{
        "B",
        "KB",
        "MB",
        "GB",
        "TB",
        "PB",
        "EB",
    };
    uint suffixIndex = 0;
    auto count = static_cast<double> (bytes);
    while (count >= 1024 && suffixIndex < 7)
    {
        suffixIndex++;
        count /= 1024;
    }

    if (count - floor (count) == 0.0)
    {
        return (boost::format ("%d%s") % count % suffixes[suffixIndex]).str ();
    }

    return (boost::format ("%.1f%s") % count % suffixes[suffixIndex]).str ();
}

FileSaver::FileSaver () = default;

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
    readerThread = std::thread (&FileSaver::entryReader, this);
}

void FileSaver::stop ()
{
    if (!running)
    {
        return;
    }

    timer.stop ();
    manager.stop ();
    running = false;
    readerThread.join ();
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
    return fileSizeService.getCurrentSizeAt (filepath);
}

bool FileSaver::isPathFinished (boost::filesystem::path& filepath)
{
    return fileSizeService.isPathFinished (filepath);
}

void FileSaver::entryReader ()
{
    while (running)
    {
        auto maybe_entry = manager.resultQueue.frontWithTimeout (std::chrono::milliseconds (300));
        if (maybe_entry.has_value ())
        {
            auto entry = maybe_entry.value ();
            fileSizeService.onFileEntry (entry);
            totalFiles += 1;
            totalKnownFiles += entry->children ().size ();
        }
    }
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
    return totalFiles;
}

unsigned long FileSaver::getTotalKnownFiles ()
{
    return totalKnownFiles;
}

double FileSaver::getFilesPerSecond ()
{
    auto timeDiff = getElapsed ();
    return static_cast<double> (totalFiles) / (static_cast<double> (timeDiff) / 1000.0);
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
