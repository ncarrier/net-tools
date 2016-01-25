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
