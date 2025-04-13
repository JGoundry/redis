// Created by Josh Goundry on 04/04/25

#pragma once

#include <vector>

#include "RespTypes.hpp"

namespace resp
{

// Request parser for the RESP2 protocol
// Currently parses requests in full, no partial requests
bool parseRespRequest( const std::vector< char >& data, resp::Request& request );

} // namespace resp
