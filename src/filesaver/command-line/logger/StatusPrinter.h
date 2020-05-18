//
// Created by Pedro Tacla Yamada on 27/02/20.
//

#ifndef FILESAVER_STATUSPRINTER_H
#define FILESAVER_STATUSPRINTER_H

#include "StatusDescr.h"

namespace filesaver
{

class StatusPrinter
{
public:
    void logStatus(const StatusDescr& statusDescr) const;
};

} // namespace filesaver

#endif // FILESAVER_STATUSPRINTER_H
