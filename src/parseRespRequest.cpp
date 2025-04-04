// Created by Josh Goundry on 04/04/25

#include "parseRespRequest.hpp"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <iostream>
#include <iterator>
#include <optional>
#include <system_error>

namespace resp
{

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

// each request will come in as an array
// "*2\r\n$3\r\nGET\r\n$5\r\nmykey\r\n"
// sizeof array: 2 \r\n command: GET \r\n sizeof bulk string: 5 \r\n
bool parseRespRequest( const std::vector< char >& data, resp::Request& request )
{
    if ( data.empty() )
        return false;

    // RESP packets use the '\r\n' delimiter
    const std::vector< char > delim = { '\\', 'r', '\\', 'n' };

    auto currentIt = data.begin();

    // 1. Check request starts with * to signify array
    if ( *currentIt != '*' )
        return false;

    currentIt = std::next( currentIt );
    std::cout << "Advanced currentIt to: " << *currentIt << '\n';

    // 2. Get the array len which comes after *
    auto delimFindIt = std::search( currentIt, data.end(), delim.begin(), delim.end() );
    if ( delimFindIt == data.end() )
        return false;

    std::optional< int > arrayLen = parseInt( currentIt.base(), delimFindIt.base() );
    if ( !arrayLen )
        return false;
    request.arrayLen = *arrayLen;

    std::cout << "Found array len: " << *arrayLen << '\n';

    // Advance iterator past the delim
    currentIt = delimFindIt;
    std::advance( currentIt, delim.size() );
    if ( currentIt == data.end() )
        return false;

    std::cout << "Advanced currentIt to: " << *currentIt << '\n';

    // 3. Get the bulk string size specifier, starts with $
    if ( *currentIt != '$' )
        return false;
    currentIt = std::next( currentIt );

    // find delim
    delimFindIt = std::search( currentIt, data.end(), delim.begin(), delim.end() );
    if ( delimFindIt == data.end() )
        return false;

    std::optional< int > commandLen = parseInt( currentIt.base(), delimFindIt.base() );
    if ( !commandLen )
        return false;

    std::cout << "Found command len: " << *arrayLen << '\n';

    // Advance iterator past the delim
    currentIt = delimFindIt;
    std::advance( currentIt, delim.size() );
    if ( currentIt == data.end() )
        return false;

    std::string command;
    command.reserve( *commandLen );

    for ( int commandLenRemaining = *commandLen;
          commandLenRemaining > 0 && currentIt != data.end();
          --commandLenRemaining, ++currentIt )
    {
        command.push_back( *currentIt );
    }

    std::cout << "Command is: " << command << '\n';

    return true;
}

} // namespace resp
