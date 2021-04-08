//
// Created by Pedro Tacla Yamada on 27/02/20.
//

#ifndef FILESAVER_STATUSDESCR_H
#define FILESAVER_STATUSDESCR_H

#include <cstdlib>
#include <string>

namespace filesaver::command_line::logger
{

/// Description of statistics for logging
class StatusDescr
{
public:
    StatusDescr (double _filesPerSecond,
                 long long int _millisecondsElapsed,
                 unsigned long _totalFiles,
                 std::string _bytesScanned);

    [[nodiscard]] double getFilesPerSecond () const;
    [[nodiscard]] long long int getMillisecondsElapsed () const;
    [[nodiscard]] unsigned long getTotalFiles () const;
    [[nodiscard]] const std::string& getBytesScanned () const;

private:
    double filesPerSecond;
    long long int millisecondsElapsed;
    unsigned long totalFiles;
    std::string bytesScanned;
};

} // namespace filesaver::command_line::logger

#endif // FILESAVER_STATUSDESCR_H
