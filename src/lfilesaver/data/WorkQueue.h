//
// Created by Pedro Tacla Yamada on 2019-08-21.
//

#ifndef FILE_SAVER_WORKQUEUE_H
#define FILE_SAVER_WORKQUEUE_H

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>

namespace filesaver::data
{

/// Provides a lock based queue
/**
 * This is a simple lock based queue.
 */
template <typename T> class WorkQueue
{
public:
    /**
     * Try to pop an element from the queue, waiting for `timeout` milliseconds
     * for it to be available.
     *
     * @param timeout Amount of time to wait for an element to be available.
     * @return Maybe an element
     */
    std::optional<T> frontWithTimeout (std::chrono::milliseconds timeout)
    {
        std::unique_lock<std::mutex> lock (criticalSection);

        while (store.empty ())
        {
            auto hasValue = conditionVariable.wait_for (lock, timeout, [&] { return !store.empty (); });

            if (!hasValue)
            {
                return {};
            }
        }

        assert (lock.owns_lock ());
        assert (!store.empty ());

        auto element = store.front ();
        store.pop ();

        return {std::move (element)};
    }

    /**
     * Pop an element from the queue, waiting for until an element is available
     * if it's empty.
     */
    T front ()
    {
        std::unique_lock<std::mutex> lock (criticalSection);

        while (store.empty ())
        {
            conditionVariable.wait (lock, [&] { return !store.empty (); });
        }

        assert (lock.owns_lock ());
        assert (!store.empty ());

        auto element = store.front ();
        store.pop ();

        return element;
    }

    /**
     * Push an element into the queue.
     */
    void push (const T& item)
    {
        std::lock_guard<std::mutex> lock (criticalSection);
        store.push (item);
        conditionVariable.notify_all ();
    }

    /**
     * Move an element into the queue.
     */
    void push (T&& item)
    {
        std::lock_guard<std::mutex> lock (criticalSection);
        store.push (item);
        conditionVariable.notify_all ();
    }

    /**
     * Get the queue size.
     */
    size_t size ()
    {
        std::lock_guard<std::mutex> lock (criticalSection);
        return store.size ();
    }

private:
    std::queue<T> store{};
    std::mutex criticalSection{};
    std::condition_variable conditionVariable{};
};

} // namespace filesaver::data

#endif // FILE_SAVER_WORKQUEUE_H
