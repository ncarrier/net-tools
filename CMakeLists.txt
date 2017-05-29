cmake_minimum_required(VERSION 2.8.4)

set(CPACK_PACKAGE_VERSION 0.1.1)

# Configure project.
set(CPACK_GENERATOR "TGZ")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)
set(CPACK_PACKAGE_NAME "enyx-net-tools")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "Enyx networking tools")
set(CPACK_PACKAGE_CONTACT "Support <support@enyx.com>")
set(CPACK_PACKAGE_VENDOR "enyx")
set(PACKAGE_URL "http://www.enyx.com")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION /usr/lib64/pkgconfig)

include(GNUInstallDirs)

project(${CPACK_PACKAGE_NAME})

find_package(Threads REQUIRED)
set(Boost_NO_BOOST_CMAKE ON)
find_package(Boost 1.40 REQUIRED COMPONENTS thread system regex program_options date_time regex)

if (WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601) # Windows 7
endif ()

add_definitions(-DBOOST_FOREACH_NO_RVALUE_DETECTION=1)

set(CMAKE_CXX_FLAGS "-Wall ${CMAKE_CXX_FLAGS}")

include_directories(${Boost_INCLUDE_DIR})

link_directories(${Boost_LIBRARY_DIR})

add_subdirectory(src)

include(CPack)
cpack_add_component(enyx-net-tools REQUIRED)
