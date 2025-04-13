#include "RespTypes.hpp"
#include "parseRespRequest.hpp"

#include "gtest/gtest.h"

namespace
{

TEST( ParseRespRequestTest, EmptyData )
{
    const std::vector< char > data;
    resp::Request request;

    const bool success = resp::parseRespRequest( data, request );

    ASSERT_EQ( success, false );
}

} // anonymous namespace
