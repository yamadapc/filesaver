//
// Created by Pedro Tacla Yamada on 27/02/20.
//

#include "StatusDescr.h"

namespace filesaver::command_line::logger
{

StatusDescr::StatusDescr (double _filesPerSecond,
                          long long int _millisecondsElapsed,
                          unsigned long _totalFiles,
                          size_t _storageQueueSizee,
                          size_t _inMemoryEntryCount,
                          const std::string& _bytesScaned)
    : filesPerSecond (_filesPerSecond),
      millisecondsElapsed (_millisecondsElapsed),
      totalFiles (_totalFiles),
      storageQueueSize (_storageQueueSizee),
      inMemoryEntryCount (_inMemoryEntryCount),
      bytesScanned (_bytesScaned)
{
}

double StatusDescr::getFilesPerSecond () const
{
    return filesPerSecond;
}

long long int StatusDescr::getMillisecondsElapsed () const
{
    return millisecondsElapsed;
}

unsigned long StatusDescr::getTotalFiles () const
{
    return totalFiles;
}

size_t StatusDescr::getStorageQueueSize () const
{
    return storageQueueSize;
}

size_t StatusDescr::getInMemoryEntryCount () const
{
    return inMemoryEntryCount;
}

const std::string& StatusDescr::getBytesScanned () const
{
    return bytesScanned;
}

} // namespace filesaver::command_line::logger
