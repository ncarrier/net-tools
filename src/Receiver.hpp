#pragma once

#include <stdint.h>
#include <vector>

#include <boost/thread/thread.hpp>

#include "Configuration.hpp"
#include "Statistics.hpp"
#include "Socket.hpp"

namespace enyx {
namespace tcp_tester {

class Receiver
{
public:
    enum { BUFFER_SIZE = 16 * 1024 };

public:
    Receiver(const Configuration & configuration,
             Socket & socket, Statistics & statistics);

    ~Receiver();

private:
    typedef std::vector<uint8_t> buffer_type;

private:
    void
    loop();

private:
    Statistics & statistics_;
    Socket & socket_;
    Configuration::Verify verify_;
    buffer_type buffer_;
    boost::thread thread_;
};

} // namespace tcp_tester
} // namespace enyx
