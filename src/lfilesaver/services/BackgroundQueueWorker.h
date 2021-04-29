//
// Created by Pedro Tacla Yamada on 2/7/20.
//


#pragma once

#include <atomic>
#include <spdlog/spdlog.h>
#include <thread>

#include "../data/WorkQueue.h"
#include "../statsd/StatsdClient.h"
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

    ~BackgroundQueueWorker ()
    {
        spdlog::info ("Shutting-down BackgroundQueueWorker({})", m_workerTag);
        stop();
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

            {
                auto start = std::chrono::steady_clock::now ();
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

                    auto now = std::chrono::steady_clock::now ();
                    auto timeSinceStart = std::chrono::duration_cast<std::chrono::milliseconds> (now - start).count ();
                    if (timeSinceStart > 400)
                    {
                        break;
                    }
                }

                if (work.size ())
                {
                    auto end = std::chrono::steady_clock::now ();
                    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds> (end - start).count ();

                    // m_statsdClient.count ("work_copied", static_cast<int> (work.size ()), 1);
                    // m_statsdClient.timing ("work_copy_time", static_cast<unsigned int> (elapsed));

                    spdlog::debug ("Copying took some time workerTag={} items={} copyTimeMs={}",
                                   m_workerTag,
                                   work.size (),
                                   elapsed);
                }
            }

            if (work.size ())
            {
                auto start = std::chrono::steady_clock::now ();
                auto workSize = work.size ();
                spdlog::debug ("Background worker taking batch workerTag={} items={} this={}",
                               m_workerTag,
                               workSize,
                               static_cast<void*> (this));
                handler (work);
                auto end = std::chrono::steady_clock::now ();
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds> (end - start).count ();
                spdlog::debug ("DONE Background worker finished batch workerTag={} items={} timeMs={}",
                               m_workerTag,
                               workSize,
                               elapsed);
            }
        }
    }

    virtual void handler (std::vector<T>)
    {
    }

private:
    // statsd::StatsdClient m_statsdClient{"127.0.0.1", 8125, "filesaver.", 10};
    std::string m_workerTag{"UNNAMED"};
    std::shared_ptr<data::WorkQueue<T>> m_workQueue;
};

} // namespace filesaver::services
