//
// Created by Pedro Tacla Yamada on 2019-08-21.
//

#include "Worker.h"

namespace filesaver
{

Worker::Worker (data::WorkQueue<boost::filesystem::path>& queue, data::WorkQueue<std::shared_ptr<FileEntry>>& rqueue)
    : resultQueue (rqueue), workQueue (queue)
{
}

void Worker::start ()
{
    running = true;

    while (running)
    {
        auto maybeFile = workQueue.frontWithTimeout (std::chrono::milliseconds (10));
        if (maybeFile)
        {
            auto file = maybeFile.value ();
            processEntry (file);
        }
    }
}

void Worker::processEntry (boost::filesystem::path& file)
{
    if (file == "/System/Volumes/Data")
    {
        std::shared_ptr<FileEntry> fileEntry = std::make_shared<FileEntry> ();
        fileEntry->size = 0;
        fileEntry->filepath = file;
        fileEntry->dev = 0;
        fileEntry->ino = 0;
        fileEntry->isFinished = true;
        fileEntry->type = FileType::unknown;
        resultQueue.push (fileEntry);
        return;
    }

    auto fileEntry = FileEntry::fromPath (file);
    filesProcessed += 1;

    auto& children = fileEntry->children ();
    resultQueue.push (fileEntry);

    for (const auto& child : children)
    {
        workQueue.push (child);
    }
}

unsigned long Worker::getFilesProcessed ()
{
    return filesProcessed;
}

void Worker::stop ()
{
    running = false;
}

} // namespace filesaver
