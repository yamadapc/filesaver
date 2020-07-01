//
// Created by Pedro Tacla Yamada on 1/7/20.
//


#pragma once

#include <atomic>
#include <functional>
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace filesaver::server
{

struct Stats
{
    double filesPerSecond;
    long long int millisecondsElapsed;
    unsigned long totalFiles;
};

void to_json (nlohmann::json& j, const Stats& stats);

class Server
{
public:
    Server (std::function<Stats ()> statsProvider, std::function<off_t (const std::string& path)> sizeProvider);

    void start ();
    void stop ();

private:
    std::function<Stats ()> m_statsProvider;
    std::function<off_t (const std::string&)> m_sizeProvider;
    httplib::Server m_server;
};

} // namespace filesaver::server
