//
// Created by Pedro Tacla Yamada on 2/7/20.
//


#pragma once

#include <atomic>
#include <thread>

#include "../data/WorkQueue.h"

namespace filesaver::services
{

template <typename T> class BackgroundWorker
{
public:
    using Handler = std::function<void(std::vector<T>)>;
    explicit BackgroundWorker (Handler handler)
        : m_handler (handler), m_workQueue (std::make_shared<data::WorkQueue<T>> ())
    {
        start ();
    }

    BackgroundWorker (Handler handler, std::shared_ptr<data::WorkQueue<T>> workQueue)
        : m_handler (handler), m_workQueue (workQueue)
    {
        start ();
    }

    ~BackgroundWorker ()
    {
        stop ();
    }

    void start ()
    {
        if (!m_running)
        {
            m_running = true;
            m_workThread = std::thread (&BackgroundWorker::run, this);
        }
    }

    void stop ()
    {
        if (m_running)
        {
            m_running = false;
            m_workThread.join ();
        }
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

private:
    void run ()
    {
        while (m_running)
        {
            std::vector<T> work;

            while (m_workQueue->size ())
            {
                auto pair = m_workQueue->frontWithTimeout (std::chrono::milliseconds (100));
                if (pair.has_value ())
                {
                    work.push_back (pair.value ());
                }
                else
                {
                    break;
                }
            }

            m_handler (std::move (work));
        }
    }

    Handler m_handler;
    std::shared_ptr<data::WorkQueue<T>> m_workQueue;
    std::thread m_workThread;
    std::atomic<bool> m_running = false;
};

} // namespace filesaver::services
