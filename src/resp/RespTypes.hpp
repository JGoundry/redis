// Created by Josh Goundry on 04/04/25

#pragma once

#include <cstdint>
#include <ostream>
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
    Request() : arrayLen( 0 ), cmd( COMMAND::UNKNOWN ) {}
    Request( const int arrayLen, const COMMAND cmd, const std::vector< std::string >& args )
        : arrayLen( arrayLen ), cmd( cmd ), args( args ) {}

    bool operator==( const Request& rhs ) const
    {
        if ( arrayLen != rhs.arrayLen )
            return false;

        if ( cmd != rhs.cmd )
            return false;

        return args == rhs.args;
    }

    int arrayLen;
    COMMAND cmd;
    std::vector< std::string > args;
};

COMMAND stringToCommand( const std::string& commandStr );
std::string commandToString( const COMMAND command );
int commandToArrayLen( const COMMAND command );

std::ostream& operator<<( std::ostream& os, const Request& request );

} // namespace resp
