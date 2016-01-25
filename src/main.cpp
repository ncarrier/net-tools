#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include "Executable.hpp"

int
main(int argc, char **argv)
{
    try
    {
        enyx::tcp_tester::Executable::run(argc, argv);
        return EXIT_SUCCESS;
    }
    catch (const std::exception & e)
    {
        std::cerr << "Failed because " << e.what() << "." << std::endl;
    }
    catch (...)
    {
        std::cerr << "Failed because an unknow error occured." << std::endl;
    }

    return EXIT_FAILURE;
}

