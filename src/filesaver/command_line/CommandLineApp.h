//
// Created by Pedro Tacla Yamada on 2019-09-05.
//

#ifndef FILE_SAVER_COMMANDLINEAPP_H
#define FILE_SAVER_COMMANDLINEAPP_H

#include <chrono>

#include <lfilesaver/FileSaver.h>
#include "logger/StatusPrinter.h"

namespace filesaver
{

class CommandLineApp
{
public:
    int main (int argc, char** argv);

private:
    void logCurrentStatus (FileSaver& fileSaver, const std::chrono::steady_clock::time_point& startTime) const;

    StatusPrinter statusPrinter;
};

} // namespace filesaver

#endif // FILE_SAVER_COMMANDLINEAPP_H
