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
