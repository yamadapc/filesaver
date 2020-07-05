//
// Created by Pedro Tacla Yamada on 2/7/20.
//

#include <catch2/catch.hpp>

#include <lfilesaver/services/BackgroundQueueWorker.h>

class MockQueueWorker : public filesaver::services::BackgroundQueueWorker<int>
{
public:
    void handler (std::vector<int>) override
    {
    }
};

TEST_CASE ("BackgroundQueueWorker can be constructed")
{
    MockQueueWorker backgroundWorker;
    REQUIRE (backgroundWorker.getWorkQueue ()->size () == 0);
}

TEST_CASE ("BackgroundQueueWorker can be stopped")
{
    MockQueueWorker backgroundWorker;
    backgroundWorker.start ();
    backgroundWorker.stop ();
    REQUIRE (backgroundWorker.getWorkQueue ()->size () == 0);
}
