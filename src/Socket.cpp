#include "Socket.hpp"

#include <iostream>

#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <boost/asio/deadline_timer.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

namespace enyx {
namespace tcp_tester {

namespace ao = boost::asio;
namespace pt = boost::posix_time;

Socket::Socket(const Configuration & configuration)
    : io_service_(), socket_(io_service_)
{
    const ao::ip::tcp::endpoint e(resolve(configuration.endpoint));

    switch (configuration.mode)
    {
        default:
        case Configuration::CLIENT:
            connect(e);
            break;
        case Configuration::SERVER:
            listen(e, pt::seconds(10));
            break;
    }


    if (configuration.windows)
    {
        ao::socket_base::receive_buffer_size r(configuration.windows);
        socket_.set_option(r);

        ao::socket_base::send_buffer_size s(configuration.windows);
        socket_.set_option(s);
    }

    socket_.non_blocking(configuration.non_blocking_io);
}

ao::ip::tcp::endpoint
Socket::resolve(const std::string & s)
{
    boost::smatch m;
    {
        boost::regex r("([^:]+):([^:]+)");
        if (! boost::regex_match(s, m, r))
        {
            std::ostringstream error;
            error << "invalid endpoint'" << s << "'";
            throw std::runtime_error(error.str());
        }
    }

    ao::ip::tcp::resolver::query q(m.str(1), m.str(2));

    ao::ip::tcp::resolver r(io_service_);
    return *r.resolve(q);
}

void
Socket::connect(const ao::ip::tcp::endpoint & e)
{
    socket_.open(e.protocol());
    socket_.connect(e);
}

void
Socket::listen(const ao::ip::tcp::endpoint & e,
               const boost::posix_time::time_duration & timeout)
{
    std::cout << "Waiting " << timeout
              << " for client to connect" << std::endl;

    boost::system::error_code failure;

    // Schedule an asynchronous accept.
    ao::ip::tcp::acceptor a(io_service_, e);
    on_accept on_accept = { failure };
    a.async_accept(socket_, on_accept);

    // And an asynchronous wait.
    ao::deadline_timer t(io_service_, timeout);
    on_timeout on_timeout = { failure };
    t.async_wait(on_timeout);

    // Wait for any to complete.
    // If the timer completed, the failure variable
    // will be set to timed_out.
    io_service_.run_one();

    if (failure)
        throw boost::system::system_error(failure);
}

void
Socket::shutdown_send()
{
    boost::system::error_code failure;
    socket_.shutdown(ao::ip::tcp::socket::shutdown_send, failure);
}

void
Socket::close()
{
    boost::system::error_code failure;
    socket_.close(failure);
}

} // namespace tcp_tester
} // namespace enyx
