//
// Created by Pedro Tacla Yamada on 1/7/20.
//


#pragma once

#include <atomic>
#include <fruit/fruit.h>
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "../services/filesize/FileSizeService.h"
#include "./util/StatsProvider.h"

namespace filesaver::server
{

using namespace filesaver::services;

class Server
{
public:
    INJECT (Server (StatsProvider* statsProvider, FileSizeService* fileSizeService));

    void start ();
    void stop ();

private:
    StatsProvider* m_statsProvider;
    FileSizeService* m_fileSizeService;
    httplib::Server m_server;
};

} // namespace filesaver::server
