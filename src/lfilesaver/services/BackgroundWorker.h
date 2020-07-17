//
// Created by Pedro Tacla Yamada on 3/7/20.
//


#pragma once

#include <atomic>
#include <thread>

namespace filesaver::services
{

class BackgroundWorker
{
public:
    BackgroundWorker ()
    {
    }

    virtual ~BackgroundWorker ()
    {
        stop ();
    }

    BackgroundWorker (const BackgroundWorker&) = delete;
    BackgroundWorker& operator= (const BackgroundWorker&) = delete;
    BackgroundWorker (BackgroundWorker&&) = delete;
    BackgroundWorker& operator= (BackgroundWorker&&) = delete;

    void start ()
    {
        if (!!m_running)
        {
            return;
        }

        m_running = true;
        m_workThread = std::thread (&BackgroundWorker::run, this);
    }

    void stop ()
    {
        if (!m_running)
        {
            return;
        }

        m_running = false;
        m_workThread.join ();
    }

    virtual void run ()
    {
    }

    bool isRunning ()
    {
        return m_running;
    }

private:
    std::thread m_workThread;
    std::atomic<bool> m_running = false;
};

} // namespace filesaver::services