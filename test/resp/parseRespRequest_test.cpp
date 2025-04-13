// Created by Josh Goundry on 13/04/25

#include "RespTypes.hpp"
#include "parseRespRequest.hpp"

#include "gtest/gtest.h"

namespace
{

inline std::vector< char > strToCharVec( const std::string_view& str )
{
    return std::vector< char >( str.begin(), str.end() );
}

struct ParseRespRequestParam
{
    std::vector< char > data;
    resp::Request expectedRequest;
    bool expectedSuccess;
};

class ParseRespRequestTest : public testing::TestWithParam< ParseRespRequestParam >
{
};

const std::vector< ParseRespRequestParam > params = {
    {
        // Empty data
        {},
        {},
        false,
    },
    {
        // Valid get request
        strToCharVec( "*2\\r\\n$3\\r\\nGET\\r\\n$3\\r\\nFoo\\r\\n" ),
        resp::Request{
            2,
            resp::COMMAND::GET,
            { "Foo" },
        },
        true,
    },
};

TEST_P( ParseRespRequestTest, Test )
{
    const ParseRespRequestParam param = GetParam();

    resp::Request request;

    const bool success = resp::parseRespRequest( param.data, request );

    ASSERT_EQ( success, param.expectedSuccess );
    ASSERT_EQ( request, param.expectedRequest );
}

INSTANTIATE_TEST_SUITE_P(, ParseRespRequestTest, testing::ValuesIn( params ) );

} // anonymous namespace
