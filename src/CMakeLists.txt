add_executable(nx-iperf
    main.cpp
    Error.hpp
    Error.cpp
    Size.hpp
    Size.cpp
    Socket.hpp
    Socket.cpp
    Statistics.hpp
    Statistics.cpp
    Executable.hpp
    Executable.cpp
    Configuration.hpp
    Configuration.cpp
    BandwidthThrottle.hpp
    BandwidthThrottle.cpp
    Application.hpp
    Application.cpp)

target_link_libraries(nx-iperf
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS
        nx-iperf
    DESTINATION
        ${CMAKE_INSTALL_BINDIR}
    COMPONENT tools)
