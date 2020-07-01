//
// Created by Pedro Tacla Yamada on 1/7/20.
//

#include "Server.h"

namespace filesaver::server
{

void to_json (nlohmann::json& j, const filesaver::server::Stats& stats)
{
    j = {{"filesPerSecond", stats.filesPerSecond},
         {"millisecondsElapsed", stats.millisecondsElapsed},
         {"totalFiles", stats.totalFiles}};
}

Server::Server (std::function<Stats ()> statsProvider, std::function<off_t (const std::string&)> sizeProvider)
    : m_statsProvider (statsProvider), m_sizeProvider (sizeProvider)
{
}

void Server::start ()
{
    m_server.Get ("/", [&](const httplib::Request&, httplib::Response& res) {
        spdlog::info ("GET index page, returning statistics");

        auto stats = m_statsProvider ();
        nlohmann::json response = stats;
        res.status = 200;
        res.set_content (response.dump (), "application/json");
    });

    m_server.Get ("/(.+)", [&](const httplib::Request& req, httplib::Response& res) {
        spdlog::info ("GET directory statistics");
        std::string path = req.matches[1];

        nlohmann::json response;
        response["path"] = path;
        response["size"] = m_sizeProvider (path);
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
