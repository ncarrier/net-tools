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

#include "Sender.hpp"

#include <boost/asio/buffer.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread_time.hpp>

namespace enyx {
namespace tcp_tester {

namespace ao = boost::asio;
namespace pt = boost::posix_time;

Sender::Sender(const Configuration & configuration,
               Socket & socket, Statistics & statistics)
        : statistics_(statistics),
          socket_(socket),
          sent_bytes_count_limit_(to_limit(configuration.size)),
          buffer_(generate_buffer()),
          slice_bytes_count_(to_bytes_count_per_slice(configuration)),
          slice_duration_(to_slice_duration(configuration)),
          thread_(boost::bind(&Sender::loop, this))
{ }

Sender::~Sender()
{
    thread_.join();
}

void
Sender::stop()
{
    sent_bytes_count_limit(0);
}

uint64_t
Sender::to_limit(uint64_t value)
{
    if (value)
        return value;

    return -1;
}

uint64_t
Sender::to_bytes_count_per_slice(const Configuration & c)
{
    if (! c.sending_bandwidth || ! c.frequency_sending_bandwidth)
        return -1;

    return c.sending_bandwidth / c.frequency_sending_bandwidth;
}

boost::posix_time::time_duration
Sender::to_slice_duration(const Configuration & c)
{
    return pt::seconds(1) / c.frequency_sending_bandwidth;
}

Sender::buffer_type
Sender::generate_buffer()
{
    buffer_type b;
    b.reserve(BUFFER_SIZE);

    for (std::size_t i = 0; i != BUFFER_SIZE; ++i)
        b.push_back(uint8_t(i));

    return b;
}

void
Sender::loop()
{
    // Let's prevent false sharing by using local variable.
    Size sent_bytes_count = Size();

    const boost::system_time start = boost::get_system_time();

    boost::system_time last_slice = start;
    uint64_t slice_bytes_count = slice_bytes_count_;
    while (sent_bytes_count < sent_bytes_count_limit())
    {
        std::size_t offset = sent_bytes_count % BUFFER_SIZE;

        boost::system::error_code failure;

        size_t count = std::min(BUFFER_SIZE - offset, slice_bytes_count);
        count = socket_.write_some(ao::buffer(&buffer_[offset], count),
                                   failure);

        if (failure == ao::error::try_again)
            continue;

        if (failure)
            sent_bytes_count_limit(0);

        sent_bytes_count += count;
        slice_bytes_count -= count;
        if (slice_bytes_count == 0)
        {
            slice_bytes_count = slice_bytes_count_;
            last_slice += slice_duration_;
            boost::this_thread::sleep(last_slice);
        }
    }

    statistics_.sent_bytes_count = sent_bytes_count;
    statistics_.transmission_duration = boost::get_system_time() - start;

    socket_.shutdown_send();
}

void
Sender::sent_bytes_count_limit(uint64_t limit)
{
    boost::mutex::scoped_lock l(mutex_);
    sent_bytes_count_limit_ = limit;
}

uint64_t
Sender::sent_bytes_count_limit()
{
    boost::mutex::scoped_lock l(mutex_);
    return sent_bytes_count_limit_;
}

} // namespace tcp_tester
} // namespace enyx

