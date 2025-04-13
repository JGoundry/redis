// Created by Josh Goundry on 09/04/25

#include "RespTypes.hpp"

#include <unordered_map>

namespace resp
{

COMMAND stringToCommand( const std::string& commandStr )
{
    const std::unordered_map< std::string, COMMAND > commandMap = {
        { "GET", COMMAND::GET },
        { "SET", COMMAND::SET },
    };

    const auto& findIt = commandMap.find( commandStr );
    if ( findIt != commandMap.end() )
    {
        return findIt->second;
    }

    return COMMAND::UNKNOWN;
}

std::string commandToString( const COMMAND command )
{
    switch ( command )
    {
    case COMMAND::GET:
        return "GET";
    case COMMAND::SET:
        return "SET";
    case COMMAND::UNKNOWN:
        return "UNKNOWN";
    }
}

int commandToArrayLen( const COMMAND command )
{
    switch ( command )
    {
    case COMMAND::GET:
        return 2;
    case COMMAND::SET:
        return 3;
    case COMMAND::UNKNOWN:
        return 0;
    }
}

std::ostream& operator<<( std::ostream& os, const Request& request )
{
    os << commandToString( request.cmd );

    for ( const std::string& arg : request.args )
    {
        os << ' ' << arg;
    }

    return os;
}

} // namespace resp
