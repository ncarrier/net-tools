/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 EnyxSA
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <stdint.h>
#include <vector>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

#include "Configuration.hpp"
#include "Statistics.hpp"
#include "Socket.hpp"

namespace enyx {
namespace tcp_tester {

class Sender
{
public:
    enum { BUFFER_SIZE = 16 * 1024 };

public:
    Sender(const Configuration & configuration,
           Socket & socket, Statistics & statistics);

    ~Sender();

    void
    stop();

private:
    typedef std::vector<uint8_t> buffer_type;

private:
    static uint64_t
    to_limit(uint64_t value);

    static uint64_t
    to_bytes_count_per_slice(const Configuration & configuration);

    static boost::posix_time::time_duration
    to_slice_duration(const Configuration & configuration);

    static buffer_type
    generate_buffer();

    void
    loop();

    void
    sent_bytes_count_limit(uint64_t limit);

    uint64_t
    sent_bytes_count_limit();

private:
    Statistics & statistics_;
    Socket & socket_;
    boost::mutex mutex_;
    uint64_t slice_bytes_count_;
    boost::posix_time::time_duration slice_duration_;
    uint64_t sent_bytes_count_limit_;
    buffer_type buffer_;
    boost::thread thread_;
};

} // namespace tcp_tester
} // namespace enyx
