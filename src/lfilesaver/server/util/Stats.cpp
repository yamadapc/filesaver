//
// Created by Pedro Tacla Yamada on 14/7/20.
//

#include "Stats.h"

namespace filesaver::server
{

void to_json (nlohmann::json& j, const Stats& stats)
{
    j = {{"filesPerSecond", stats.filesPerSecond},
         {"millisecondsElapsed", stats.millisecondsElapsed},
         {"totalFiles", stats.totalFiles}};
}

} // namespace filesaver::server