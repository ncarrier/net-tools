#pragma once

#include <stdint.h>
#include <iosfwd>

#include <boost/date_time/posix_time/posix_time_types.hpp>

#include "Size.hpp"

namespace enyx {
namespace tcp_tester {

struct Statistics
{
    uint64_t checksum_error;
    Size received_bytes_count;
    boost::posix_time::time_duration reception_duration;
    Size sent_bytes_count;
    boost::posix_time::time_duration transmission_duration;
};

std::ostream &
operator<<(std::ostream & out, const Statistics & statistics);

} // namespace tcp_tester
} // namespace enyx
