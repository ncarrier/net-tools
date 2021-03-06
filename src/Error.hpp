#pragma once

#include <boost/system/error_code.hpp>

namespace enyx {
namespace tcp_tester {

namespace error {

enum Error
{
    success = 0,
    generic_fault = 1,
    unexpected_eof = 2,
    checksum_failed = 3,
    test_timeout = 4,
    unexpected_data = 5,
};

boost::system::error_code
make_error_code(Error e);

} // namespace error

const boost::system::error_category &
tcp_tester_category();

} // namespace tcp_tester
} // namespace enyx

namespace boost {
namespace system {

template<>
struct is_error_code_enum<enyx::tcp_tester::error::Error>
{ static const bool value =  true; };

} // namespace system
} // namespace boost

