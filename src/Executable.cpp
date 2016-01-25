#include "Executable.hpp"

#include <stdexcept>
#include <sstream>

#include <boost/program_options.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "Configuration.hpp"
#include "Application.hpp"

namespace enyx {
namespace tcp_tester {

namespace po = boost::program_options;
namespace pt = boost::posix_time;

namespace {

Configuration
parse_command_line(int argc, char** argv)
{
    Configuration c = {};

    po::options_description required("Required arguments");
    required.add_options()
        ("connect,c",
            po::value<std::string>(&c.endpoint),
            "Connect to following address\n")
        ("listen,l",
            po::value<std::string>(&c.endpoint),
            "Listen on following address\n");

    po::options_description optional("Optional arguments");
    optional.add_options()
        ("bandwidth,b",
            po::value<Size>(&c.sending_bandwidth),
            "Limit transmission bandwidth (e.g. 8Kb, 16Mb, 1Gb, 1GB)\n")
        ("frequency-sending-bandwidth,f",
            po::value<uint64_t>(&c.frequency_sending_bandwidth)
                ->default_value(100),
            "Sending bandwidth calculation frequency Hz\n")
        ("verify,v",
            po::value<Configuration::Verify>(&c.verify)
                ->default_value(Configuration::NONE),
            "Verify received bytes."
            "\nAccepted values:\n  none\n  first\n  all\n")
        ("windows,w",
            po::value<Size>(&c.windows),
            "Tcp socket buffer size (e.g. 8Kb, 16Mb)\n")
        ("size,s",
            po::value<Size>(&c.size),
            "Amount of data to send (e.g. 8Kb, 16Mb, 1Gb, 1GB)\n")
        ("non-blocking-io,n",
            po::value<bool>(&c.non_blocking_io)
                ->implicit_value(true),
            "Use non-blocking io\n")
        ("duration,d",
            po::value<pt::time_duration>(&c.duration)
                ->default_value(pt::not_a_date_time, "infinity"),
            "Test duration\n")
        ("help,h",
            "Print the command lines arguments\n");

    po::options_description all("Allowed options");
    all.add(required).add(optional);

    po::variables_map args;
    po::store(po::parse_command_line(argc, argv, all), args);
    po::notify(args);

    if (args.count("help"))
    {
        std::cout << all << std::endl;
        throw std::runtime_error("help is requested");
    }

    if (args.count("connect") && args.count("listen"))
        throw std::runtime_error("--connect and --listen are mutually exclusive");

    if (args["frequency-sending-bandwidth"].as<uint64_t>() == 0)
        throw std::runtime_error("invalid --frequency-sending-bandwidth");

    if (args.count("connect"))
        c.mode = Configuration::CLIENT;
    else if (args.count("listen"))
        c.mode = Configuration::SERVER;
    else
        throw std::runtime_error("--connect or --listen are required");

    return c;
}

} // namespace

namespace Executable {

void
run(int argc, char** argv)
{
    Application::run(parse_command_line(argc, argv));
}

}

} // namespace tcp_tester
} // namespace enyx
