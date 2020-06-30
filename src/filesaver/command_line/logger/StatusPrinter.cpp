//
// Created by Pedro Tacla Yamada on 27/02/20.
//

#include <spdlog/spdlog.h>

#include "StatusPrinter.h"

namespace filesaver::command_line::logger
{

void StatusPrinter::logStatus (const StatusDescr& statusDescr) const
{
    spdlog::info ("{} - {} files scanned - {}/second - {}ms elapsed",
                  statusDescr.getBytesScanned (),
                  statusDescr.getTotalFiles (),
                  statusDescr.getFilesPerSecond (),
                  statusDescr.getMillisecondsElapsed ());
}

} // namespace filesaver::command_line::logger
