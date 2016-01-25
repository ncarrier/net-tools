#include "Receiver.hpp"

#include <boost/asio/buffer.hpp>
#include <boost/thread/thread_time.hpp>

namespace enyx {
namespace tcp_tester {

namespace ao = boost::asio;

Receiver::Receiver(const Configuration & configuration,
                   Socket & socket, Statistics & statistics)
        : statistics_(statistics),
          socket_(socket),
          verify_(configuration.verify),
          buffer_(BUFFER_SIZE),
          thread_(boost::bind(&Receiver::loop, this))
{ }

Receiver::~Receiver()
{
    thread_.join();
}

void
Receiver::loop()
{
    boost::system::error_code failure;

    const boost::system_time start = boost::get_system_time();
    Size received_bytes_count = Size();
    uint64_t checksum_error = 0;
    while (! failure)
    {
        size_t count = socket_.read_some(ao::buffer(buffer_), failure);
        if (failure == ao::error::try_again)
        {
            failure.clear();
            continue;
        }

        uint8_t expected_byte = received_bytes_count;
        received_bytes_count += count;

        switch (verify_)
        {
            case Configuration::FIRST:
                if (expected_byte != buffer_[0])
                    ++ checksum_error;
                break;
            case Configuration::ALL:
                for (uint64_t i = 0; i != count; ++i)
                    if (buffer_[i] != expected_byte++)
                        ++ checksum_error;
                break;
            default:
                break;
        }
    }
    statistics_.checksum_error = checksum_error;
    statistics_.received_bytes_count = received_bytes_count;
    statistics_.reception_duration = boost::get_system_time() - start;
}

} // namespace tcp_tester
} // namespace enyx
