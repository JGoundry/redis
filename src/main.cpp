// Created by Josh Goundry on 29/03/25

#include "ConnectionServer.hpp"

#include <iostream>
#include <stdexcept>


int main( int argc, char* argv[] )
{
    try
    {
        std::cout << "Starting server...\n";

        ConnectionServer connectionServer( 6379 );
        connectionServer.StartAsync();
    }
    catch ( std::runtime_error& e )
    {
        std::cout << "Caught runtime error: " << e.what() << '\n';
    }
    catch ( std::exception& e )
    {
        std::cout << "Caught exception: " << e.what() << '\n';
    }
    catch ( ... )
    {
        std::cout << "Caught unknown exception\n";
    }

    return 0;
}
