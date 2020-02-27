//
// Created by Pedro Tacla Yamada on 27/02/20.
//

#include <iostream>
#include <thread>

#include "StatusPrinter.h"

namespace filesaver
{

void StatusPrinter::logStatus (const StatusDescr& statusDescr) const
{
    std::cout << "Working... " << statusDescr.getBytesScanned () << " - " << statusDescr.getTotalFiles ()
              << " files scanned"
              << " - " << statusDescr.getFilesPerSecond () << "/second"
              << " - " << statusDescr.getMillisecondsElapsed () << "ms elapsed"
              << " - " << statusDescr.getStorageQueueSize () << " entries waiting to be stored"
              << " - " << statusDescr.getInMemoryEntryCount () << " entries held in memory" << std::endl;
    std::cout.flush ();
}

} // namespace filesaver
