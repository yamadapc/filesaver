// MIT License
//
// Copyright (c) 2017 Vincent Thiery
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
// Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Modified from https://github.com/vthiery/cpp-statsd-client
#include "UDPSender.h"

#include <spdlog/spdlog.h>
#include <utility>

namespace filesaver::statsd
{

UDPSender::UDPSender (std::string host, const uint16_t port, const std::optional<uint64_t> batchsize) noexcept
    : m_host (std::move (host)), m_port (port)
{
    // If batching is on, use a dedicated thread to send every now and then
    if (batchsize)
    {
        // Thread' sleep duration between batches
        // TODO: allow to input this
        constexpr unsigned int batchingWait{1000U};

        m_batching = true;
        m_batchsize = batchsize.value ();

        // Define the batching thread
        m_batchingThread = std::thread ([this, batchingWait] {
            while (!m_mustExit)
            {
                std::deque<std::string> stagedMessageQueue;

                std::unique_lock<std::mutex> batchingLock (m_batchingMutex);
                m_batchingMessageQueue.swap (stagedMessageQueue);
                batchingLock.unlock ();

                // Flush the queue
                while (!stagedMessageQueue.empty ())
                {
                    sendToDaemon (stagedMessageQueue.front ());
                    stagedMessageQueue.pop_front ();
                }

                // Wait before sending the next batch
                std::this_thread::sleep_for (std::chrono::milliseconds (batchingWait));
            }
        });
    }
}

UDPSender::~UDPSender ()
{
    if (m_batching)
    {
        m_mustExit = true;
        m_batchingThread.join ();
    }

    if (m_socket >= 0)
    {
        close (m_socket);
        m_socket = -1;
    }
}

void UDPSender::setConfig (const std::string& host, const uint16_t port) noexcept
{
    m_host = host;
    m_port = port;

    m_isInitialized = false;

    if (m_socket >= 0)
    {
        close (m_socket);
    }
    m_socket = -1;
}

void UDPSender::send (const std::string& message) noexcept
{
    // If batching is on, accumulate messages in the queue
    if (m_batching)
    {
        std::unique_lock<std::mutex> batchingLock (m_batchingMutex);
        if (m_batchingMessageQueue.empty () || m_batchingMessageQueue.back ().length () > m_batchsize)
        {
            m_batchingMessageQueue.push_back (message);
        }
        else
        {
            std::rbegin (m_batchingMessageQueue)->append ("\n").append (message);
        }
    }
    else
    {
        sendToDaemon (message);
    }
}

std::optional<std::string> UDPSender::errorMessage () const noexcept
{
    return m_errorMessage;
}

bool UDPSender::initialize () noexcept
{
    using namespace std::string_literals;

    if (m_isInitialized)
    {
        return true;
    }

    // Connect the socket
    m_socket = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (m_socket == -1)
    {
        m_errorMessage = "Could not create socket, err="s + std::strerror (errno);
        return false;
    }

    std::memset (&m_server, 0, sizeof (m_server));
    m_server.sin_family = AF_INET;
    m_server.sin_port = htons (m_port);

    if (inet_aton (m_host.c_str (), &m_server.sin_addr) == 0)
    {
        // An error code has been returned by inet_aton

        // Specify the criteria for selecting the socket address structure
        struct addrinfo hints;
        std::memset (&hints, 0, sizeof (hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_DGRAM;

        // Get the address info using the hints
        struct addrinfo* results = nullptr;
        const int ret{getaddrinfo (m_host.c_str (), nullptr, &hints, &results)};
        if (ret != 0)
        {
            // An error code has been returned by getaddrinfo
            close (m_socket);
            m_socket = -1;
            m_errorMessage = "getaddrinfo failed: error="s + std::to_string (ret) + ", msg=" + gai_strerror (ret);
            return false;
        }

        // Copy the results in m_server
        struct sockaddr_in* host_addr = reinterpret_cast<struct sockaddr_in*> (results->ai_addr);
        std::memcpy (&m_server.sin_addr, &host_addr->sin_addr, sizeof (struct in_addr));

        // Free the memory allocated
        freeaddrinfo (results);
    }

    m_isInitialized = true;
    return true;
}

void UDPSender::sendToDaemon (const std::string& message) noexcept
{
    // Can't send until the sender is initialized
    if (!initialize ())
    {
        return;
    }

    // Try sending the message
    const long int ret{sendto (m_socket,
                               message.data (),
                               message.size (),
                               0,
                               reinterpret_cast<struct sockaddr*> (&m_server),
                               sizeof (m_server))};
    if (ret == -1)
    {
        using namespace std::string_literals;
        m_errorMessage =
            "sendto server failed: host="s + m_host + ":" + std::to_string (m_port) + ", err=" + std::strerror (errno);
        spdlog::error ("UDPSender - Sending UDP message failed {}", errno);
    }
    else
    {
        spdlog::debug ("UDPSender - Sent UDP message");
    }
}

} // namespace filesaver::statsd
