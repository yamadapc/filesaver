//
// Created by Pedro Tacla Yamada on 27/02/20.
//

#ifndef FILESAVER_STATUSDESCR_H
#define FILESAVER_STATUSDESCR_H

#include <cstdlib>
#include <string>

namespace filesaver
{

class StatusDescr
{
public:
    StatusDescr (double filesPerSecond,
                 long long int millisecondsElapsed,
                 unsigned long totalFiles,
                 size_t storageQueueSize,
                 size_t inMemoryEntryCount,
                 const std::string& bytesScanned);

    [[nodiscard]] double getFilesPerSecond () const;
    [[nodiscard]] long long int getMillisecondsElapsed () const;
    [[nodiscard]] unsigned long getTotalFiles () const;
    [[nodiscard]] size_t getStorageQueueSize () const;
    [[nodiscard]] size_t getInMemoryEntryCount () const;
    [[nodiscard]] const std::string& getBytesScanned () const;

private:
    double filesPerSecond;
    long long int millisecondsElapsed;
    unsigned long totalFiles;
    size_t storageQueueSize;
    size_t inMemoryEntryCount;
    std::string bytesScanned;
};

} // namespace filesaver

#endif // FILESAVER_STATUSDESCR_H
