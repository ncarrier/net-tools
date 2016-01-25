Description
===========
This package contains various tools used
to generate and verify network traffic.

Build requirements
==================
* CMake
* boost devel

Usage
=====

nx-iperf
======
Wait for a tcp connection on _loopback_ port _12345_ and generate _25Mb/s_ traffic:

    $nx-iperf --listen 127.0.0.1:12345 --bandwidth 25Mb

Connect on _loopback_ port _12345_ and generate _10Gb/s_ traffic while checking received data:

    $nx-iperf --connect 127.0.0.1:12345 --bandwidth 10Gb --verify=all


