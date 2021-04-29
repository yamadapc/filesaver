//
// Created by Pedro Tacla Yamada on 2019-09-05.
//

#ifndef FILE_SAVER_COMMANDLINEAPP_H
#define FILE_SAVER_COMMANDLINEAPP_H

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <chrono>
#include <spdlog/spdlog.h>

#include <lfilesaver/FileSaver.h>
#include <lfilesaver/factory/FileSaverFactory.h>

#include "CommandLineOptions.h"
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
    int handleListCategories (FileSaverFactory& fileSaverFactory) const;
    int handleCategoryFind (FileSaverFactory& fileSaverFactory, CategoryFindOptions categoryFindOptions) const;
    int handleScan (FileSaverFactory& fileSaverFactory, ScanCommandOptions scanCommandOptions) const;
};

} // namespace filesaver::command_line

#endif // FILE_SAVER_COMMANDLINEAPP_H
