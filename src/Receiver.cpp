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

#include "Receiver.hpp"

#include <boost/asio/buffer.hpp>
#include <boost/thread/thread_time.hpp>

namespace enyx {
namespace tcp_tester {

namespace ao = boost::asio;

Receiver::Receiver(const Configuration & configuration,
                   Socket & socket, Statistics & statistics)
        : statistics_(statistics),
          socket_(socket),
          verify_(configuration.verify),
          buffer_(BUFFER_SIZE),
          thread_(boost::bind(&Receiver::loop, this))
{ }

Receiver::~Receiver()
{
    thread_.join();
}

void
Receiver::loop()
{
    boost::system::error_code failure;

    const boost::system_time start = boost::get_system_time();
    Size received_bytes_count = Size();
    uint64_t checksum_error = 0;
    while (! failure)
    {
        size_t count = socket_.read_some(ao::buffer(buffer_), failure);
        if (failure == ao::error::try_again)
        {
            failure.clear();
            continue;
        }

        uint8_t expected_byte = received_bytes_count;
        received_bytes_count += count;

        switch (verify_)
        {
            case Configuration::FIRST:
                if (expected_byte != buffer_[0])
                    ++ checksum_error;
                break;
            case Configuration::ALL:
                for (uint64_t i = 0; i != count; ++i)
                    if (buffer_[i] != expected_byte++)
                        ++ checksum_error;
                break;
            default:
                break;
        }
    }
    statistics_.checksum_error = checksum_error;
    statistics_.received_bytes_count = received_bytes_count;
    statistics_.reception_duration = boost::get_system_time() - start;
}

} // namespace tcp_tester
} // namespace enyx
