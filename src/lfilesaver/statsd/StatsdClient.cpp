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
#include "StatsdClient.h"

namespace filesaver::statsd
{

StatsdClient::StatsdClient (const std::string& host,
                            const uint16_t port,
                            const std::string& prefix,
                            const std::optional<uint64_t> batchSize) noexcept
    : m_prefix (prefix), m_sender (host, port, batchSize)
{
    // Initialize the random generator to be used for sampling
    std::srand (static_cast<unsigned int> (time (nullptr)));
}

void StatsdClient::setConfig (const std::string& host, const uint16_t port, const std::string& prefix) noexcept
{
    m_prefix = prefix;
    m_sender.setConfig (host, port);
}

std::optional<std::string> StatsdClient::errorMessage () const noexcept
{
    return m_sender.errorMessage ();
}

void StatsdClient::decrement (const std::string& key, const float frequency) const noexcept
{
    return count (key, -1, frequency);
}

void StatsdClient::increment (const std::string& key, const float frequency) const noexcept
{
    return count (key, 1, frequency);
}

void StatsdClient::count (const std::string& key, const int delta, const float frequency) const noexcept
{
    return send (key, delta, "c", frequency);
}

void StatsdClient::gauge (const std::string& key, const unsigned int value, const float frequency) const noexcept
{
    return send (key, static_cast<int> (value), "g", frequency);
}

void StatsdClient::timing (const std::string& key, const unsigned int ms, const float frequency) const noexcept
{
    return send (key, static_cast<int> (ms), "ms", frequency);
}

void StatsdClient::send (const std::string& key, const int value, const std::string& type, const float frequency) const
    noexcept
{
    const auto isFrequencyOne = [](const float freq) noexcept
    {
        constexpr float epsilon{0.0001f};
        return std::fabs (freq - 1.0f) < epsilon;
    };

    // Test if one should send or not, according to the frequency rate
    if (!isFrequencyOne (frequency))
    {
        if (frequency < static_cast<float> (std::rand ()) / static_cast<float> (RAND_MAX))
        {
            return;
        }
    }

    // Prepare the buffer, with a sampling rate if specified different from 1.0f
    char buffer[256];
    if (isFrequencyOne (frequency))
    {
        // Sampling rate is 1.0f, no need to specify it
        std::snprintf (buffer, sizeof (buffer), "%s%s:%d|%s", m_prefix.c_str (), key.c_str (), value, type.c_str ());
    }
    else
    {
        // Sampling rate is different from 1.0f, hence specify it
        std::snprintf (buffer,
                       sizeof (buffer),
                       "%s%s:%d|%s|@%.2f",
                       m_prefix.c_str (),
                       key.c_str (),
                       value,
                       type.c_str (),
                       static_cast<double> (frequency));
    }

    // Send the message via the UDP sender
    m_sender.send (buffer);
}

} // namespace filesaver::statsd