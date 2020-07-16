//
// Created by Pedro Tacla Yamada on 14/7/20.
//


#pragma once

#include "Stats.h"

namespace filesaver::server
{

class StatsProvider
{
public:
    virtual ~StatsProvider () = default;
    virtual Stats getStats () = 0;
};

} // namespace filesaver::server