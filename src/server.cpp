// Created by Josh Goundry on 27/03/25

#include <iostream>
#include "boost/container/flat_set.hpp"

inline void testBoost()
{
    boost::container::flat_set<std::string> flatSet = {"hello", "boost"};
    for (const auto& string : flatSet)
        std::cout << string << " ";
    std::cout << '\n';
}

int main(int argc, char* argv[])
{
    std::cout << "Starting server...\n";

    testBoost();

    return 0;
}
