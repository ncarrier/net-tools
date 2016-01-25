#include "Statistics.hpp"

#include <string>
#include <iostream>
#include <sstream>

#include <boost/date_time/posix_time/posix_time.hpp>

namespace enyx {
namespace tcp_tester {

namespace pt = boost::posix_time;

namespace {

std::string
compute_bandwith(Size bytes_count,
                 const pt::time_duration & duration)
{
    std::ostringstream out;

    bytes_count /= duration.total_milliseconds();
    bytes_count *= 1000;

    out << bytes_count << "/s";

    return out.str();
}

} // anonymous namespace

std::ostream &
operator<<(std::ostream & out, const Statistics & statistics)
{
    return out << "checksum_error: "
               << statistics.checksum_error << "\n"
               << "received_bytes_count: "
               << statistics.received_bytes_count << "\n"
               << "reception_bandwith: "
               << compute_bandwith(statistics.received_bytes_count,
                                   statistics.reception_duration) << "\n"
               << "sent_bytes_count: "
               << statistics.sent_bytes_count << "\n"
               << "transmission_bandwidth: "
               << compute_bandwith(statistics.sent_bytes_count,
                                   statistics.transmission_duration)
               << std::endl;
}

} // namespace tcp_tester
} // namespace enyx
