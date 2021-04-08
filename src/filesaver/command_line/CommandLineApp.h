//
// Created by Pedro Tacla Yamada on 2019-09-05.
//

#ifndef FILE_SAVER_COMMANDLINEAPP_H
#define FILE_SAVER_COMMANDLINEAPP_H

#include <chrono>

#include <lfilesaver/FileSaver.h>
#include "logger/StatusPrinter.h"

#define CLI_LOG_DELAY 300

/// Command-line application namespace
namespace filesaver::command_line
{

/// Command-line entry-point.
/**
 * This provides the filesaver command-line app.
 *
 * Usage is as follows:
 *
 * ```
 * filesaver [...options] [...INPUT_FILE]
 *
 * General options:
 *   -h [ --help ]         print this help message
 *   --num-workers arg     The number of worker threads to use
 *   --input-file arg      input file
 * ```
 *
 * Multiple targets may be passed in. The output of the command-line app won't
 * be particularly useful.
 */
class CommandLineApp
{
public:
    int main (int argc, char** argv);

private:
    void logCurrentStatus (FileSaver& fileSaver, const std::chrono::steady_clock::time_point& startTime) const;

    logger::StatusPrinter statusPrinter;
};

} // namespace filesaver

#endif // FILE_SAVER_COMMANDLINEAPP_H
