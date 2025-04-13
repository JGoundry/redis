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

constexpr bool debug = false;

// RESP packets use the '\r\n' delimiter
constexpr std::array< char, 4 > delim = { '\\', 'r', '\\', 'n' };

std::optional< int > parseInt( const char* first, const char* last )
{
    int num;

    const auto& [ptr, ec] = std::from_chars( first, last, num );
    if ( ec != std::errc() )
    {
        if constexpr ( debug )
        {
            std::cerr << "Error `parseInt`: "
                      << std::make_error_code( ec ).message() << '\n';
        }
        return std::nullopt;
    }

    return num;
}

template< typename Iterator >
inline Iterator findDelimIt( const Iterator& currentIt, const Iterator& end )
{
    return std::search( currentIt, end, delim.begin(), delim.end() );
}

template< typename Iterator >
bool advanceCurrentItPastDelim( Iterator& currentIt, const Iterator& end, const Iterator& delimFindIt )
{
    if ( std::distance( currentIt, end ) < delim.size() )
    {
        if constexpr ( debug )
        {
            std::cerr << "Error `advanceCurrentItPastDelim`: "
                      << "distance between currentIt and end too small for delim\n";
        }
        return false;
    }

    currentIt = delimFindIt;
    std::advance( currentIt, delim.size() );
    return true;
}

template< typename Iterator >
std::optional< int > getLengthAfterPrefix( Iterator& currentIt,
                                           const Iterator& end,
                                           const char prefix,
                                           const bool advanceItPastDelim )
{
    static constexpr std::string_view errorPrefix = "Error `getLengthAfterPrefix`: ";

    if ( *currentIt != prefix )
    {
        if constexpr ( debug )
        {
            std::cerr << errorPrefix
                      << "currentIt not at prefix\n";
        }
        return std::nullopt;
    }

    currentIt = std::next( currentIt );

    // find delim
    auto delimFindIt = findDelimIt( currentIt, end );
    if ( delimFindIt == end )
    {
        if constexpr ( debug )
        {
            std::cerr << errorPrefix
                      << "could not find delim\n";
        }
        return std::nullopt;
    }

    std::optional< int > len = parseInt( currentIt.base(), delimFindIt.base() );
    if ( !len )
    {
        return std::nullopt;
    }

    if ( advanceItPastDelim &&
         !advanceCurrentItPastDelim( currentIt, end, delimFindIt ) )
    {
        return std::nullopt;
    }

    return len;
}

template< typename Iterator >
inline std::optional< int > getBulkStrLength( Iterator& currentIt,
                                              const Iterator& end,
                                              const bool advanceItPastDelim )
{
    return getLengthAfterPrefix( currentIt, end, '$', advanceItPastDelim );
}

template< typename Iterator >
inline std::optional< int > getArrayLength( Iterator& currentIt,
                                            const Iterator& end,
                                            const bool advanceItPastDelim )
{
    return getLengthAfterPrefix( currentIt, end, '*', advanceItPastDelim );
}

template< typename Iterator >
std::optional< std::string > getBulkStr( Iterator& currentIt, const Iterator& end, int bulkStrLen )
{
    if ( std::distance( currentIt, end ) < bulkStrLen )
    {
        if constexpr ( debug )
        {
            std::cerr << "Error `getBulkStr`: "
                      << "distance between currentIt and end too small for bulk str\n";
        }
        return std::nullopt;
    }

    const std::string bulkStr( currentIt.base(), bulkStrLen );

    std::advance( currentIt, bulkStrLen );

    return bulkStr;
}

template< typename Iterator >
std::optional< std::string > getBulkStr( Iterator& currentIt, const Iterator& end )
{
    std::optional< int > bulkStrLen = getBulkStrLength( currentIt, end, /*advanceItPastDelim=*/true );
    if ( !bulkStrLen )
        return std::nullopt;

    return getBulkStr( currentIt, end, *bulkStrLen );
}

} // anonymous namespace

// each request will come in as an array
// "*2\r\n$3\r\nGET\r\n$5\r\nmykey\r\n"
// arr len: 2 '\r\n' cmd: GET '\r\n' bulk str len: 5 '\r\n' bulk str: mykey '\r\n'
bool parseRespRequest( const std::vector< char >& data, resp::Request& request )
{
    if ( data.empty() )
        return false;

    static constexpr std::string_view errorPrefix = "Error `parseRespRequest`: ";

    auto currentIt = data.begin();
    const auto& endIt = data.end();

    // ----------------------------------------------------
    // 1. Get the array len which comes after *
    // ----------------------------------------------------

    const std::optional< int > arrayLen = getArrayLength( currentIt,
                                                          endIt,
                                                          /*advanceItPastDelim=*/true );
    if ( !arrayLen )
        return false;

    request.arrayLen = *arrayLen;
    int remainingArrayLen = *arrayLen;

    if constexpr ( debug )
    {
        std::cout << "Array length: " << request.arrayLen << '\n';
        std::cout << "CurrentIt:    " << *currentIt << '\n';
    }

    // ----------------------------------------------------
    // 2. Get the bulk string size specifier, starts with $
    // ----------------------------------------------------

    const std::optional< std::string > commandStr = getBulkStr( currentIt, endIt );
    if ( !commandStr )
        return false;

    request.cmd = resp::stringToCommand( *commandStr );
    if ( request.cmd == resp::COMMAND::UNKNOWN )
    {
        if constexpr ( debug )
        {
            std::cerr << errorPrefix
                      << "unknown command\n";
        }
        return false;
    }

    // Check array len matches expected length from command
    if ( resp::commandToArrayLen( request.cmd ) != request.arrayLen )
    {
        if constexpr ( debug )
        {
            std::cerr << errorPrefix
                      << "array length does not match expected length from command\n";
        }
        return false;
    }

    std::cout << "CurrentIt:    " << *currentIt << '\n';

    // If the command was the correct size specified by bulk string length
    // then the currentIt should be the begging of the '\r\n' delim
    auto delimFindIt = findDelimIt( currentIt, endIt );
    if ( currentIt != delimFindIt )
    {
        if constexpr ( debug )
        {
            std::cerr << errorPrefix
                      << "current it not at delim\n";
        }
        return false;
    }

    --remainingArrayLen;

    if constexpr ( debug )
    {
        std::cout << "Command:        " << commandToString( request.cmd ) << '\n';
        std::cout << "CurrentIt:      " << *currentIt << '\n';
        std::cout << "Remaining args: " << remainingArrayLen << '\n';
    }

    // ----------------------------------------------------
    // 3. Loop to collect remaining args if there is any
    // ----------------------------------------------------

    while ( remainingArrayLen > 0 )
    {
        advanceCurrentItPastDelim( currentIt, endIt, delimFindIt );
        if ( currentIt == endIt )
            return false;

        const std::optional< std::string > bulkStr = getBulkStr( currentIt, endIt );
        if ( !bulkStr )
            return false;

        request.args.emplace_back( *bulkStr );

        --remainingArrayLen;

        // If the command was the correct size specified by bulk string length
        // then the currentIt should be the begging of the '\r\n' delim
        auto delimFindIt = findDelimIt( currentIt, endIt );
        if ( currentIt != delimFindIt )
            return false;

        if constexpr ( debug )
        {
            std::cout << "Arg:            " << request.args.back() << '\n';
            std::cout << "CurrentIt:      " << *currentIt << '\n';
            std::cout << "Remaining args: " << remainingArrayLen << '\n';
        }
    }

    return true;
}

} // namespace resp
