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

#include "UDPSender.h"
#include <cstdlib>
#include <optional>
#include <string>

namespace filesaver::statsd
{

/*!
 *
 * Statsd client
 *
 * This is the Statsd client, exposing the classic methods
 * and relying on a UDP sender for the actual sending.
 *
 * The sampling frequency can be input, as well as the
 * batching size. The latter is optional and shall not be
 * set if batching is not desired.
 *
 */
class StatsdClient
{
public:
    //!@name Constructor and destructor
    //!@{

    //! Constructor
    StatsdClient (const std::string& host,
                  const uint16_t port,
                  const std::string& prefix,
                  const std::optional<uint64_t> batchSize = std::nullopt) noexcept;

    //!@}

    //!@name Methods
    //!@{

    //! Sets a configuration { host, port, prefix }
    void setConfig (const std::string& host, const uint16_t port, const std::string& prefix) noexcept;

    //! Returns the error message as an optional std::string
    std::optional<std::string> errorMessage () const noexcept;

    //! Increments the key, at a given frequency rate
    void increment (const std::string& key, const float frequency = 1.0f) const noexcept;

    //! Increments the key, at a given frequency rate
    void decrement (const std::string& key, const float frequency = 1.0f) const noexcept;

    //! Adjusts the specified key by a given delta, at a given frequency rate
    void count (const std::string& key, const int delta, const float frequency = 1.0f) const noexcept;

    //! Records a gauge for the key, with a given value, at a given frequency rate
    void gauge (const std::string& key, const unsigned int value, const float frequency = 1.0f) const noexcept;

    //! Records a timing for a key, at a given frequency
    void timing (const std::string& key, const unsigned int ms, const float frequency = 1.0f) const noexcept;

    //! Send a value for a key, according to its type, at a given frequency
    void send (const std::string& key, const int value, const std::string& type, const float frequency = 1.0f) const
        noexcept;

    //!@}

private:
    //! The prefix to be used for metrics
    std::string m_prefix;

    //! The UDP sender to be used for actual sending
    mutable UDPSender m_sender;
};

} // namespace filesaver::statsd
