//
// Created by Pedro Tacla Yamada on 2019-08-20.
//

#define CATCH_CONFIG_RUNNER

#include <cstdlib>
#include <catch2/catch.hpp>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

int main (int argc, char* argv[])
{
    spdlog::warn ("Logs will be written to filesaver_test.log");
    auto testLogger = spdlog::basic_logger_mt ("FileSaverTest", "filesaver_test.log");
    spdlog::set_default_logger (testLogger);

    return Catch::Session ().run (argc, argv);
}
