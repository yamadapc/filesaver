//
// Created by Pedro Tacla Yamada on 27/02/20.
//

#include "StatusDescr.h"

namespace filesaver
{

StatusDescr::StatusDescr (double filesPerSecond,
                          long long int millisecondsElapsed,
                          unsigned long totalFiles,
                          size_t storageQueueSize,
                          size_t inMemoryEntryCount,
                          const std::string& bytesScanned)
    : filesPerSecond (filesPerSecond),
      millisecondsElapsed (millisecondsElapsed),
      totalFiles (totalFiles),
      storageQueueSize (storageQueueSize),
      inMemoryEntryCount (inMemoryEntryCount),
      bytesScanned (bytesScanned)
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

} // namespace filesaver
