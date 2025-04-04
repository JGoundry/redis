// Created by Josh Goundry on 04/04/25

#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace resp
{

enum class COMMAND : uint8_t
{
    GET,
    SET,
    UNKNOWN,
};

// COMMAND stringToCommand( const std::string& commandStr )
// {
//     const std::unordered_map< std::string, COMMAND > commandMap = {
//         { "GET", COMMAND::GET },
//         { "SET", COMMAND::SET },
//     };

//     const auto& findIt = commandMap.find( commandStr );
//     if ( findIt != commandMap.end() )
//     {
//         return findIt->second;
//     }

//     return COMMAND::UNKNOWN;
// }

struct Request
{
    int arrayLen;
    COMMAND cmd;
    std::vector< std::string > args;
};


} // namespace resp
