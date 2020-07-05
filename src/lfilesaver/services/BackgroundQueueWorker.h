//
// Created by Pedro Tacla Yamada on 2/7/20.
//


#pragma once

#include <atomic>
#include <spdlog/spdlog.h>
#include <thread>

#include "../data/WorkQueue.h"
#include "./BackgroundWorker.h"

namespace filesaver::services
{

const unsigned long DEFAULT_QUEUE_TIMEOUT_MS = 100;

template <typename T> class BackgroundQueueWorker : public BackgroundWorker
{
public:
    BackgroundQueueWorker () : m_workQueue (std::make_shared<data::WorkQueue<T>> ())
    {
    }

    explicit BackgroundQueueWorker (std::string workerTag)
        : m_workerTag (workerTag), m_workQueue (std::make_shared<data::WorkQueue<T>> ())
    {
    }

    BackgroundQueueWorker (std::string workerTag, std::shared_ptr<data::WorkQueue<T>> workQueue)
        : m_workerTag (workerTag), m_workQueue (workQueue)
    {
    }

    void push (const T& item)
    {
        m_workQueue->push (item);
    }

    void push (T&& item)
    {
        m_workQueue->push (item);
    }

    std::shared_ptr<data::WorkQueue<T>> getWorkQueue ()
    {
        return m_workQueue;
    }

    void run () override
    {
        while (isRunning ())
        {
            std::vector<T> work;

            while (m_workQueue->size ())
            {
                auto pair = m_workQueue->frontWithTimeout (std::chrono::milliseconds (DEFAULT_QUEUE_TIMEOUT_MS));
                if (pair.has_value ())
                {
                    work.push_back (pair.value ());
                }
                else
                {
                    break;
                }
            }

            if (work.size ())
            {
                spdlog::info ("workerTag={} items={} Background worker taking batch", m_workerTag, work.size ());
                handler (std::move (work));
            }

            std::this_thread::sleep_for (std::chrono::milliseconds (DEFAULT_QUEUE_TIMEOUT_MS * 4));
        }
    }

    virtual void handler (std::vector<T>)
    {
    }

private:
    std::string m_workerTag{"UNNAMED"};
    std::shared_ptr<data::WorkQueue<T>> m_workQueue;
};

} // namespace filesaver::services
