/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 EnyxSA
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "Application.hpp"

#include <csignal>
#include <iostream>

#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "Socket.hpp"
#include "SignalHandler.hpp"
#include "Sender.hpp"
#include "Receiver.hpp"
#include "Statistics.hpp"

namespace enyx {
namespace tcp_tester {

namespace {

void
send(const Configuration & configuration,
     Socket & socket,
     Statistics & statistics)
{
    Sender sender(configuration, socket, statistics);

    if (! configuration.duration.is_not_a_date_time())
    {
        std::cout << "Sending for " << configuration.duration
                  << "." << std::endl;
        boost::this_thread::sleep(configuration.duration);
        sender.stop();

        std::cout << "Shutdown transmission." << std::endl;
    }

    else if (configuration.size)
        std::cout << "Sending ~" << configuration.size
                  << "." << std::endl;

    else
    {
        std::cout << "Press CTRL-C to shutdown transmission."
                  << std::endl;

        SignalHandler().wait();
        sender.stop();

        std::cout << "Shutdown transmission." << std::endl;
    }
}

} // anonymous namespace

void
Application::run(const Configuration & configuration)
{
    std::cout << configuration << std::endl;

    Statistics statistics = {};

    {
        // Connects or waits for a connection
        // as requested by configuration.
        Socket socket(configuration);

        // Start data reception in a dedicated thread.
        Receiver receiver(configuration, socket, statistics);

        // Starts data sending in a dedicated thread.
        send(configuration, socket, statistics);

        std::cout << "Waiting for peer shutdown." << std::endl;
    }

    std::cout << "\n" << statistics << std::endl;

    if (statistics.checksum_error)
        throw std::runtime_error("checksum error(s)");

    if (statistics.received_bytes_count == 0)
        throw std::runtime_error("didn't received anything");
}

} // namespace tcp_tester
} // namespace enyx
