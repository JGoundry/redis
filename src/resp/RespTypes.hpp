// Created by Josh Goundry on 04/04/25

#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace resp
{

enum class COMMAND : uint8_t
{
    GET,
    SET,
    UNKNOWN,
};

struct Request
{
    int arrayLen;
    COMMAND cmd;
    std::vector< std::string > args;
};

COMMAND stringToCommand( const std::string& commandStr );
std::string commandToString( const COMMAND command );

} // namespace resp
