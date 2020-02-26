//
// Created by Pedro Tacla Yamada on 2019-08-21.
//

#include "gtest/gtest.h"

#include <future>
#include <iostream>
#include <thread>

#include "../../src/data/WorkQueue.h"

template <typename T> bool isReady (std::future<T>& future)
{
    return future.wait_for (std::chrono::seconds (0)) == std::future_status::ready;
}

TEST (WorkQueueTest, CanPushItemsThenPopThemOut)
{
    filesaver::WorkQueue<std::string> workQueue;
    EXPECT_EQ (workQueue.size (), 0);
    workQueue.push ("Something interesting");
    EXPECT_EQ (workQueue.size (), 1);
    EXPECT_EQ (workQueue.front (), "Something interesting");
    EXPECT_EQ (workQueue.size (), 0);
}

TEST (WorkQueueTest, CanWaitUntilItemsAreAdded)
{
    filesaver::WorkQueue<std::string> workQueue;
    workQueue.push ("Amazing 1");
    workQueue.push ("Amazing 2");

    {
        auto future1 = std::async ([&] {
            usleep (100000);
            auto f = workQueue.front ();
            return f;
        });

        auto future2 = std::async ([&] {
            usleep (200000);
            auto f = workQueue.front ();
            return f;
        });

        auto future3 = std::async ([&] {
            usleep (300000);
            auto f = workQueue.front ();
            return f;
        });

        usleep (210000);

        EXPECT_TRUE (isReady (future1));
        EXPECT_TRUE (isReady (future2));
        EXPECT_FALSE (isReady (future3));
        EXPECT_EQ (future1.get (), "Amazing 1");
        EXPECT_EQ (future2.get (), "Amazing 2");
        EXPECT_EQ (workQueue.size (), 0);

        usleep (300000);
        EXPECT_FALSE (isReady (future3));

        workQueue.push ("Amazing 3");
        usleep (100000);
        EXPECT_EQ (workQueue.size (), 0);
        EXPECT_TRUE (isReady (future3));
        EXPECT_EQ (future3.get (), "Amazing 3");
    }
}
