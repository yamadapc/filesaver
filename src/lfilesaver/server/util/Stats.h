//
// Created by Pedro Tacla Yamada on 14/7/20.
//


#pragma once

#include <nlohmann/json.hpp>

namespace filesaver::server
{

struct Stats
{
    double filesPerSecond;
    long long int millisecondsElapsed;
    unsigned long totalFiles;
};

void to_json (nlohmann::json& j, const Stats& stats);

}