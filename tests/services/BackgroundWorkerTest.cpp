//
// Created by Pedro Tacla Yamada on 2/7/20.
//

#include <catch2/catch.hpp>

#include <lfilesaver/services/BackgroundWorker.h>

TEST_CASE ("BackgroundWorker can be constructed")
{
    using namespace filesaver::services;
    BackgroundWorker<int> backgroundWorker{[](std::vector<int>) {}};
    REQUIRE (backgroundWorker.getWorkQueue ()->size () == 0);
}

TEST_CASE ("BackgroundWorker can be stopped")
{
    using namespace filesaver::services;
    BackgroundWorker<int> backgroundWorker{[](std::vector<int>) {}};
    backgroundWorker.stop ();
    REQUIRE (backgroundWorker.getWorkQueue ()->size () == 0);
}
