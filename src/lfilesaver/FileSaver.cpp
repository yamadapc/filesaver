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

    if (hasStorage ())
    {
        storageService->createTables ();
        storageThread = std::thread (&FileSaver::entryWriter, this);
    }
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

    if (hasStorage ())
    {
        spdlog::info ("Storing results");
        storing = true;
        storageThread.join ();
    }
}

void FileSaver::setupDefaultStorage ()
{
    storageService = std::make_unique<LevelDbStorageService> ("filesaver.db");
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
    std::unique_lock<std::mutex> lock{criticalSection};
    return getCurrentSizeAtWithoutLock (filepath);
}

off_t FileSaver::getCurrentSizeAtWithoutLock (const std::string& filepath)
{
    auto totalSizeIt = totalSizes.find (filepath);

    if (totalSizeIt == totalSizes.end ())
    {
        return 0;
    }

    return totalSizeIt->second;
}

bool FileSaver::isPathFinished (boost::filesystem::path& filepath)
{
    std::unique_lock<std::mutex> lock{criticalSection};
    const auto& filepathStr = filepath.string ();
    auto pendingIt = pendingChildren.find (filepathStr);

    if (pendingIt == pendingChildren.end ())
    {
        return false;
    }

    return pendingIt->second <= 0;
}

void FileSaver::entryReader ()
{
    unsigned long iterations = 0;

    while (running)
    {
        while (manager.resultQueue.size () > 0)
        {
            auto entry = manager.resultQueue.front ();

            // This is made to minimize contention on the lock. Result queue will block waiting on the result queue lock
            // and on there being no results.
            {
                std::unique_lock<std::mutex> lock{criticalSection};
                auto filepath = entry->filepath;
                auto filepathStr = entry->filepath.string ();

                allEntries[filepathStr] = entry;

                if (!entry->isDirectory ())
                {
                    pendingChildren[filepathStr] = 0;
                    onFinished (entry->filepath);
                }
                else
                {
                    pendingChildren[filepathStr] = entry->children ().size ();
                    if (pendingChildren[filepathStr] == 0)
                    {
                        onFinished (entry->filepath);
                    }
                }
                updateSizes (entry);
                totalFiles += 1;
                totalKnownFiles += entry->children ().size ();
            }
        }

        iterations += 1;

        std::this_thread::sleep_for (std::chrono::milliseconds (300));
    }
}

void FileSaver::entryWriter ()
{
    while (running)
    {
        {
            // This is made to minimize contention on the lock. Rather than holding it while writing to the DB,
            // we'll copy the entries to store onto a buffer then release the lock.
            std::vector<std::shared_ptr<FileEntry>> entriesToStore;
            {
                std::unique_lock<std::mutex> lock{criticalSection};
                while (storageQueue.size () > 0)
                {
                    auto entry = storageQueue.front ();
                    entriesToStore.push_back (entry);
                }
            }

            for (auto& entry : entriesToStore)
            {
                storageService->insertEntry (*entry);
            }

            spdlog::info ("Flushing {0} entries", entriesToStore.size ());
        }

        std::this_thread::sleep_for (std::chrono::milliseconds (1000));
    }
}

void FileSaver::onFinished (const boost::filesystem::path& filepath)
{
    const auto& filepathStr = filepath.string ();
    auto entry = allEntries[filepathStr];
    entry->isFinished = true;
    allEntries.erase (filepathStr);

    if (hasStorage ())
    {
        storageQueue.push (std::move (entry));
    }

    if (filepath.has_parent_path ())
    {
        auto parentPath = filepath.parent_path ();
        const auto& parentPathStr = parentPath.string ();

        pendingChildren[parentPathStr] -= 1;
        if (pendingChildren[parentPathStr] <= 0)
        {
            onFinished (parentPath);
        }
    }
}

void FileSaver::addSize (const boost::filesystem::path& path, off_t sizeDiff)
{
    auto parentIt = totalSizes.find (path.string ());
    if (parentIt != totalSizes.end ())
    {
        totalSizes[path.string ()] += sizeDiff;
    }
    else
    {
        totalSizes[path.string ()] = sizeDiff;
    }
}

void FileSaver::updateSizes (const std::shared_ptr<FileEntry>& entry)
{
    auto previousSize = getCurrentSizeAtWithoutLock (entry->filepath.string ());
    auto sizeDiff = entry->size - previousSize;
    onFileSizeChanged (entry->filepath, sizeDiff);
}

void FileSaver::onFileSizeChanged (const boost::filesystem::path& filepath, off_t sizeDiff)
{
    addSize (filepath, sizeDiff);

    // This is faster than using .has_parent_path or .parent_path because it'll
    // do less copying/alloc.
    boost::filesystem::path path = {filepath};
    do
    {
        path = path.remove_filename ();
        addSize (path, sizeDiff);
    } while (!path.empty ());
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

size_t FileSaver::getStorageQueueSize ()
{
    return storageQueue.size ();
}

size_t FileSaver::getInMemoryEntryCount ()
{
    return allEntries.size ();
}

bool FileSaver::hasStorage ()
{
    return storageService != nullptr;
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
