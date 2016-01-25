#pragma once

#include <stdint.h>
#include <string>
#include <iosfwd>

#include <boost/date_time/posix_time/posix_time_types.hpp>

#include "Size.hpp"

namespace enyx {
namespace tcp_tester {

struct Configuration
{
    enum Mode { CLIENT, SERVER } mode;
    std::string endpoint;
    Size sending_bandwidth;
    uint64_t frequency_sending_bandwidth;
    enum Verify { NONE, FIRST, ALL } verify;
    Size windows;
    Size size;
    boost::posix_time::time_duration duration;
    bool non_blocking_io;
};

std::istream &
operator>>(std::istream & in, Configuration::Verify & verify);

std::ostream &
operator<<(std::ostream & out, const Configuration::Verify & verify);

std::ostream &
operator<<(std::ostream & out, const Configuration::Mode & mode);

std::ostream &
operator<<(std::ostream & out, const Configuration & configuration);

} // namespace tcp_tester
} // namespace enyx
