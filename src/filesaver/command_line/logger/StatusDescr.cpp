//
// Created by Pedro Tacla Yamada on 27/02/20.
//

#include "StatusDescr.h"

namespace filesaver::command_line::logger
{

StatusDescr::StatusDescr (double _filesPerSecond,
                          long long int _millisecondsElapsed,
                          unsigned long _totalFiles,
                          const std::string& _bytesScaned)
    : filesPerSecond (_filesPerSecond),
      millisecondsElapsed (_millisecondsElapsed),
      totalFiles (_totalFiles),
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

const std::string& StatusDescr::getBytesScanned () const
{
    return bytesScanned;
}

} // namespace filesaver::command_line::logger
