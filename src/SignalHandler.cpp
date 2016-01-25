#include "SignalHandler.hpp"

#include <unistd.h>

namespace enyx {
namespace tcp_tester {

namespace {

volatile std::sig_atomic_t exit_requested_;

} // anonymous namespace

void
SignalHandler::wait()
{
    while (! exit_requested_)
        ::pause();
}

void
SignalHandler::handle_signal(int)
{ exit_requested_ = 1; }

} // namespace tcp_tester
} // namespace enyx


