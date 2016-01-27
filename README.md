[![Build Status](https://travis-ci.org/Enyx-SA/net-tools.svg?branch=master)](https://travis-ci.org/Enyx-SA/net-tools)

Description
===========
This package contains various tools used
to generate and verify network traffic.

Build
=====

Requirements
------------
* CMake
* boost devel

Steps
------------
On linux/BSD:

    $mkdir build && cd build
    $cmake ..
    $make -j`nproc`

On Windows:

    >mkdir build && cd build
    >cmake -G "Visual Studio 14 2015 Win64"
    >cmake --build . --config Release

Usage
=====

nx-iperf
--------
Wait for a tcp connection on _loopback_ port _12345_ and generate _25Mb/s_ traffic:

    $nx-iperf --listen 127.0.0.1:12345 --tx-bandwidth 25Mb --size 1Gb

Connect on _loopback_ port _12345_ and generate _10Mb/s_ traffic while checking received data:

    $nx-iperf --connect 127.0.0.1:12345 --tx-bandwidth 10Mb --verify=all --size 256Mb

Connect on _loopback_ port _12345_ and generate _10Mb/s_ traffic while capping reception to _5Mb/s_:

    $nx-iperf --connect 127.0.0.1:12345 --tx-bandwidth 10Mb --rx-bandwidth 5Mb

