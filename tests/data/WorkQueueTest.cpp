//
// Created by Pedro Tacla Yamada on 2019-08-21.
//

#include "gtest/gtest.h"

#include <future>
#include <iostream>
#include <thread>

#include "../../src/data/WorkQueue.h"

template <typename T> bool isReady(std::future<T> &future) {
  return future.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

TEST(WorkQueueTest, CanPushItemsThenPopThemOut) {
  filesaver::WorkQueue<std::string> workQueue;
  EXPECT_EQ(workQueue.size(), 0);
  workQueue.push("Something interesting");
  EXPECT_EQ(workQueue.size(), 1);
  EXPECT_EQ(workQueue.front(), "Something interesting");
  EXPECT_EQ(workQueue.size(), 0);
}

TEST(WorkQueueTest, CanWaitUntilItemsAreAdded) {
  filesaver::WorkQueue<std::string> workQueue;
  workQueue.push("Amazing 1");
  workQueue.push("Amazing 2");

  {
    auto future1 = std::async([&] {
      usleep(100000);
      std::cout << "Future1 getting from queue" << std::endl;
      auto f = workQueue.front();
      std::cout << "Future1 finished" << std::endl;
      return f;
    });

    auto future2 = std::async([&] {
      usleep(200000);
      std::cout << "Future2 getting from queue" << std::endl;
      auto f = workQueue.front();
      std::cout << "Future2 finished" << std::endl;
      return f;
    });

    auto future3 = std::async([&] {
      usleep(300000);
      std::cout << "Future3 getting from queue" << std::endl;
      auto f = workQueue.front();
      std::cout << "Future3 finished" << std::endl;
      return f;
    });

    std::cout << "Workers spawned. Waiting..." << std::endl;
    usleep(210000);

    std::cout << "Running assertions" << std::endl;
    EXPECT_TRUE(isReady(future1));
    EXPECT_TRUE(isReady(future2));
    EXPECT_FALSE(isReady(future3));
    EXPECT_EQ(future1.get(), "Amazing 1");
    EXPECT_EQ(future2.get(), "Amazing 2");
    EXPECT_EQ(workQueue.size(), 0);

    usleep(300000);
    EXPECT_FALSE(isReady(future3));

    std::cout << "Pushing new value" << std::endl;
    workQueue.push("Amazing 3");
    usleep(100000);
    std::cout << "Running assertions again" << std::endl;
    EXPECT_EQ(workQueue.size(), 0);
    EXPECT_TRUE(isReady(future3));
    EXPECT_EQ(future3.get(), "Amazing 3");
  }
}
