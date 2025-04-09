// Created by Josh Goundry on 04/04/25

#include "parseRespRequest.hpp"
#include "RespTypes.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <charconv>
#include <iostream>
#include <iterator>
#include <optional>
#include <system_error>

namespace resp
{
namespace
{

// RESP packets use the '\r\n' delimiter
static constexpr std::array< char, 4 > delim = { '\\', 'r', '\\', 'n' };

std::optional< int > parseInt( const char* first, const char* last )
{
    int num;

    const auto& [ptr, ec] = std::from_chars( first, last, num );
    if ( ec != std::errc() )
    {
        const std::error_code errorCode = std::make_error_code( ec );
        std::cout << "Error: " << errorCode.message() << '\n';
        return std::nullopt;
    }

    return num;
}

template< typename Iterator >
std::optional< std::string > getCommand( Iterator& currentIt, const Iterator& end, const int commandLen )
{
    std::string command;
    command.reserve( commandLen );
    for ( int commandLenRemaining = commandLen;
          commandLenRemaining > 0 && currentIt != end;
          --commandLenRemaining, ++currentIt )
    {
        command.push_back( *currentIt );
    }

    // TODO: refactor this to check that we actually do get a full command

    return command;
}

template< typename Iterator >
inline Iterator findDelimIt( const Iterator& currentIt, const Iterator& end )
{
    return std::search( currentIt, end, delim.begin(), delim.end() );
}

template< typename Iterator >
inline bool advanceCurrentItPastDelim( Iterator& currentIt, const Iterator& end, const Iterator& delimFindIt )
{
    currentIt = delimFindIt;
    std::advance( currentIt, delim.size() );
    return currentIt != end;
}

template< typename Iterator >
std::optional< int > getLengthAfterPrefix( Iterator& currentIt,
                                           const Iterator& end,
                                           const char prefix,
                                           const bool advanceItPastDelim )
{
    if ( *currentIt != prefix )
        return std::nullopt;

    currentIt = std::next( currentIt );

    // find delim
    auto delimFindIt = findDelimIt( currentIt, end );
    if ( delimFindIt == end )
        return std::nullopt;

    std::optional< int > len = parseInt( currentIt.base(), delimFindIt.base() );
    if ( !len )
        return std::nullopt;

    if ( advanceItPastDelim &&
         !advanceCurrentItPastDelim( currentIt, end, delimFindIt ) )
        return std::nullopt;

    return len;
}

template< typename Iterator >
std::optional< int > getBulkStringLength( Iterator& currentIt,
                                          const Iterator& end,
                                          const bool advanceItPastDelim )
{
    return getLengthAfterPrefix( currentIt, end, '$', advanceItPastDelim );
}

template< typename Iterator >
std::optional< int > getArrayLength( Iterator& currentIt,
                                     const Iterator& end,
                                     const bool advanceItPastDelim )
{
    return getLengthAfterPrefix( currentIt, end, '*', advanceItPastDelim );
}

} // anonymous namespace

// each request will come in as an array
// "*2\r\n$3\r\nGET\r\n$5\r\nmykey\r\n"
// sizeof array: 2 \r\n command: GET \r\n sizeof bulk string: 5 \r\n
bool parseRespRequest( const std::vector< char >& data, resp::Request& request )
{
    if ( data.empty() )
        return false;

    auto currentIt = data.begin();
    const auto& endIt = data.end();

    // ----------------------------------------------------
    // 1. Get the array len which comes after *
    // ----------------------------------------------------

    int remainingArrayLen;
    {
        std::optional< int > arrayLen = getArrayLength( currentIt,
                                                        endIt,
                                                        /*advanceItPastDelim=*/true );
        if ( !arrayLen )
            return false;

        request.arrayLen = *arrayLen;
        remainingArrayLen = *arrayLen;

        std::cout << "Found array len: " << *arrayLen << '\n';
        std::cout << "CurrentIt: " << *currentIt << '\n';
    }

    // ----------------------------------------------------
    // 2. Get the bulk string size specifier, starts with $
    // ----------------------------------------------------

    {
        std::optional< int > commandLen = getBulkStringLength( currentIt,
                                                               endIt,
                                                               /*advanceItPastDelim=*/true );
        if ( !commandLen )
            return false;

        std::cout << "Found command len: " << *commandLen << '\n';
        std::cout << "CurrentIt: " << *currentIt << '\n';

        std::optional< std::string > commandStr = getCommand( currentIt, endIt, *commandLen );
        if ( !commandStr )
            return false;

        request.cmd = resp::stringToCommand( *commandStr );
        if ( request.cmd == resp::COMMAND::UNKNOWN )
            return false;

        std::cout << "Command is: " << *commandStr << '\n';
        std::cout << "CurrentIt: " << *currentIt << '\n';

        // If the command was the correct size specified by bulk string length
        // then the currentIt should be the begging of the '\r\n' delim
        const auto delimFindIt = findDelimIt( currentIt, endIt );
        if ( currentIt != delimFindIt )
            return false;

        --remainingArrayLen;
    }

    // ----------------------------------------------------
    // 3. Loop to collect remaining args if there is any
    // ----------------------------------------------------

    while ( remainingArrayLen )
    {

        --remainingArrayLen;
    }

    return true;
}

} // namespace resp
