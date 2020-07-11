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
#pragma once

#include <arpa/inet.h>
#include <cmath>
#include <cstring>
#include <deque>
#include <mutex>
#include <netdb.h>
#include <optional>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

namespace filesaver::statsd
{

/*!
 *
 * UDP sender
 *
 * A simple UDP sender handling batching.
 * Its configuration can be changed at runtime for
 * more flexibility.
 *
 */
class UDPSender final
{
public:
    //!@name Constructor and destructor
    //!@{

    //! Constructor
    UDPSender (std::string host, const uint16_t port, const std::optional<uint64_t> batchsize = std::nullopt) noexcept;

    //! Destructor
    ~UDPSender ();

    //!@}

    //!@name Methods
    //!@{

    //! Sets a configuration { host, port }
    void setConfig (const std::string& host, const uint16_t port) noexcept;

    //! Send a message
    void send (const std::string& message) noexcept;

    //! Returns the error message as an optional string
    std::optional<std::string> errorMessage () const noexcept;

    //!@}

private:
    // @name Private methods
    // @{

    //! Initialize the sender and returns true when it is initialized
    bool initialize () noexcept;

    //! Send a message to the daemon
    void sendToDaemon (const std::string& message) noexcept;

    //!@}

private:
    // @name State variables
    // @{

    //! Is the sender initialized?
    bool m_isInitialized{false};

    //! Shall we exit?
    bool m_mustExit{false};

    //!@}

    // @name Network info
    // @{

    //! The hostname
    std::string m_host;

    //! The port
    uint16_t m_port;

    //! The structure holding the server
    struct sockaddr_in m_server
    {
    };

    //! The socket to be used
    int m_socket{-1};

    //!@}

    // @name Batching info
    // @{

    //! Shall the sender use batching?
    bool m_batching{false};

    //! The batching size
    uint64_t m_batchsize;

    //! The queue batching the messages
    mutable std::deque<std::string> m_batchingMessageQueue;

    //! The mutex used for batching
    std::mutex m_batchingMutex;

    //! The thread dedicated to the batching
    std::thread m_batchingThread;

    //!@}

    //! Error message (optional string)
    std::optional<std::string> m_errorMessage;
};

} // namespace filesaver::statsd
