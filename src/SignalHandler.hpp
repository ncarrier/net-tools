#pragma once

#include <cstddef>
#include <csignal>

namespace enyx {
namespace tcp_tester {

class SignalHandler
{
public:
    SignalHandler()
            : old_sigint_action_()
    {
        struct sigaction new_action = {};
        new_action.sa_handler = handle_signal;
        ::sigaction(SIGINT, &new_action, &old_sigint_action_);
    }

    ~SignalHandler()
    {
        ::sigaction(SIGINT, &old_sigint_action_, NULL);
    }

    void
    wait();

private:
    static void
    handle_signal(int);

private:
    struct sigaction old_sigint_action_;
};

} // namespace tcp_tester
} // namespace enyx


