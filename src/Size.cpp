#include "Size.hpp"

#include <stdexcept>
#include <iostream>
#include <iomanip>

#include <boost/regex.hpp>

namespace enyx {
namespace tcp_tester {

namespace {

uint64_t
parse_size(const std::string & s)
{
    boost::regex r("(\\d+)\\s*([KMG]?)([bB])");
    boost::smatch m;

    if (! boost::regex_match(s, m, r))
    {
        std::ostringstream error;
        error << "size '" << s
              << "' doesn't match \\d+\\s*[KMG]?[bB]";
        throw std::runtime_error(error.str());
    }

    uint64_t size = std::atoll(m.str(1).c_str());

    if (m[2].matched)
        switch (m.str(2)[0])
        {
            case 'G':
                size *= 1024;
            case 'M':
                size *= 1024;
            case 'K':
                size *= 1024;
        }

    if (m.str(3) == "b")
        size /= 8;

    return size;
}

struct precision_setter
{
    precision_setter
        (std::ostream & out, int precision)
            : out_(out), precision_(out_.precision())
    { out_.precision(precision); }

    ~precision_setter()
    { out_.precision(precision_); }

    std::ostream & out_;
    int precision_;
};

} // anonymous namespace

std::istream &
operator>>(std::istream & in, Size & size)
{
    std::istream::sentry sentry(in);

    if (sentry)
    {
        std::string s;
        in >> s;

        static_cast<uint64_t &>(size) = parse_size(s);
    }

    return in;
}

const char * units[] = { "b", "Kb", "Mb", "Gb", "Tb", "Pb", "Eb", "Zb" };
#define UNITS_COUNT ((sizeof(units)) / sizeof(units[0]))

std::ostream &
operator<<(std::ostream & out, const Size & size)
{
    std::ostream::sentry sentry(out);
    if (sentry)
    {
        long double value = size * 8;
        uint64_t i;
        for (i = 0; i != UNITS_COUNT - 1 && value / 1024. >= 1.; ++i)
            value /= 1024.;

        precision_setter p(out, 3);
        out << value << units[i];
    }

    return out;
}

} // namespace tcp_tester
} // namespace enyx

