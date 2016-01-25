#pragma once

#include <stdint.h>
#include <iosfwd>

namespace enyx {
namespace tcp_tester {

class Size
{
public:
    operator const uint64_t &() const
    { return value; }

    operator uint64_t &()
    { return value; }

private:
    uint64_t value;
};

std::istream &
operator>>(std::istream & in, Size & size);

std::ostream &
operator<<(std::ostream & out, const Size & size);

} // namespace tcp_tester
} // namespace enyx
