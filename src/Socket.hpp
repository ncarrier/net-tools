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

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>

#include "Configuration.hpp"

namespace enyx {
namespace tcp_tester {

class Socket
{
public:
    typedef boost::asio::ip::tcp::socket socket_type;

public:
    explicit
    Socket(const Configuration & configuration);

    template<typename MutableBufferSequence>
    std::size_t
    read_some(const MutableBufferSequence & buffers,
              boost::system::error_code & failure)
    { return socket_.read_some(buffers, failure); }

    template<typename ConstBufferSequence>
    std::size_t
    write_some(const ConstBufferSequence & buffers,
               boost::system::error_code & failure)
    { return socket_.write_some(buffers, failure); }

    void
    shutdown_send();

    void
    close();

    socket_type::native_handle_type
    native_handle()
    { return socket_.native_handle(); }

private:
    struct on_accept
    {
        void operator()(const boost::system::error_code & failure)
        { result_ = failure; }

        boost::system::error_code & result_;
    };

    struct on_timeout
    {
        void operator()(const boost::system::error_code &)
        { result_ = boost::asio::error::timed_out; }

        boost::system::error_code & result_;
    };

private:
    boost::asio::ip::tcp::endpoint
    resolve(const std::string & endpoint);

    void
    connect(const boost::asio::ip::tcp::endpoint & e);

    void
    listen(const boost::asio::ip::tcp::endpoint & e,
           const boost::posix_time::time_duration & timeout);

private:
    boost::asio::io_service io_service_;
    socket_type socket_;
};

} // namespace tcp_tester
} // namespace enyx
