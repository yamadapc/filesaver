//
// Created by Pedro Tacla Yamada on 2019-08-21.
//

#ifndef FILE_SAVER_WORKQUEUE_H
#define FILE_SAVER_WORKQUEUE_H

#include <chrono>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>

template <typename T> class WorkQueue {
public:
  std::optional<T> frontWithTimeout(std::chrono::milliseconds timeout) {
    std::unique_lock<std::mutex> lock(criticalSection);

    while (store.empty()) {
      auto hasValue = conditionVariable.wait_for(
          lock, timeout, [&] { return !store.empty(); });

      if (!hasValue) {
        return {};
      }
    }

    assert(lock.owns_lock());
    assert(!store.empty());

    auto element = store.front();
    store.pop();

    lock.unlock();
    return {std::move(element)};
  }

  T front() {
    std::unique_lock<std::mutex> lock(criticalSection);

    while (store.empty()) {
      conditionVariable.wait(lock, [&] { return !store.empty(); });
    }

    assert(lock.owns_lock());
    assert(!store.empty());

    auto element = store.front();
    store.pop();

    lock.unlock();
    return std::move(element);
  }

  void push(const T &item) {
    std::unique_lock<std::mutex> lock(criticalSection);
    store.push(item);
    conditionVariable.notify_one();
  }

  void push(T &&item) {
    std::unique_lock<std::mutex> lock(criticalSection);
    store.push(item);
    conditionVariable.notify_one();
  }

  size_t size() {
    std::unique_lock<std::mutex> lock(criticalSection);
    return store.size();
  }

private:
  std::queue<T> store;
  std::mutex criticalSection;
  std::condition_variable conditionVariable;
};

#endif // FILE_SAVER_WORKQUEUE_H
