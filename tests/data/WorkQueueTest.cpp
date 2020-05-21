//
// Created by Pedro Tacla Yamada on 2019-08-21.
//

#include <catch2/catch.hpp>
#include <future>
#include <thread>

#include <lfilesaver/data/WorkQueue.h>

template <typename T> bool isReady (std::future<T>& future)
{
    return future.wait_for (std::chrono::seconds (0)) == std::future_status::ready;
}

TEST_CASE ("WorkQueueTest - CanPushItemsThenPopThemOut")
{
    filesaver::data::WorkQueue<std::string> workQueue;
    REQUIRE (workQueue.size () == 0);
    workQueue.push ("Something interesting");
    REQUIRE (workQueue.size () == 1);
    REQUIRE (workQueue.front () == "Something interesting");
    REQUIRE (workQueue.size () == 0);
}

TEST_CASE ("WorkQueueTest - CanWaitUntilItemsAreAdded")
{
    filesaver::data::WorkQueue<std::string> workQueue;
    workQueue.push ("Amazing 1");
    workQueue.push ("Amazing 2");

    {
        auto future1 = std::async ([&] {
            std::this_thread::sleep_for (std::chrono::milliseconds (100));
            auto f = workQueue.front ();
            return f;
        });

        auto future2 = std::async ([&] {
            std::this_thread::sleep_for (std::chrono::milliseconds (200));
            auto f = workQueue.front ();
            return f;
        });

        auto future3 = std::async ([&] {
            std::this_thread::sleep_for (std::chrono::milliseconds (300));
            auto f = workQueue.front ();
            return f;
        });

        std::this_thread::sleep_for (std::chrono::milliseconds (210));

        REQUIRE (isReady (future1));
        REQUIRE (isReady (future2));
        REQUIRE_FALSE (isReady (future3));
        REQUIRE (future1.get () == "Amazing 1");
        REQUIRE (future2.get () == "Amazing 2");
        REQUIRE (workQueue.size () == 0);

        std::this_thread::sleep_for (std::chrono::milliseconds (300));
        REQUIRE_FALSE (isReady (future3));

        workQueue.push ("Amazing 3");
        std::this_thread::sleep_for (std::chrono::milliseconds (100));
        REQUIRE (workQueue.size () == 0);
        REQUIRE (isReady (future3));
        REQUIRE (future3.get () == "Amazing 3");
    }
}
