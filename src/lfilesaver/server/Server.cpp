//
// Created by Pedro Tacla Yamada on 1/7/20.
//

#include "Server.h"

namespace filesaver::server
{

Server::Server (StatsProvider* statsProvider, FileSizeService* fileSizeService)
    : m_statsProvider (statsProvider), m_fileSizeService (fileSizeService)
{
}

void Server::start ()
{
    m_server.Get ("/statistics", [&](const httplib::Request&, httplib::Response& res) {
        auto stats = m_statsProvider->getStats ();
        nlohmann::json response = stats;
        res.status = 200;
        res.set_content (response.dump (), "application/json");
    });

    m_server.Get ("/file_sizes", [&](const httplib::Request& req, httplib::Response& res) {
        nlohmann::json response;

        if (0 == req.get_param_value_count ("path"))
        {
            response["error"] = "Missing required querystring parameter ?path=...";
            res.status = 400;
            res.set_content (response.dump (), "application/json");
            return;
        }

        auto path = req.get_param_value ("path");
        response["path"] = path;
        auto size = m_fileSizeService->getCurrentSizeAt (path);
        response["size"] = size;
        res.status = 200;
        res.set_content (response.dump (), "application/json");
    });

    spdlog::info ("Listening on 127.0.0.1:9510");
    m_server.listen ("127.0.0.1", 9510);
}

void Server::stop ()
{
    m_server.stop ();
}

} // namespace filesaver::server
