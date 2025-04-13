// Created by Josh Goundry on 30/03/25

#pragma once

#include "RespTypes.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>

#include <memory>
#include <optional>

namespace network
{

// boost::asio tcp connection handler for the RESP protocol
class RespHandler : public std::enable_shared_from_this< RespHandler >
{
    struct Private
    {
        explicit Private() = default;
    };

public:
    RespHandler( const Private, boost::asio::io_context& ctx );

    static std::shared_ptr< RespHandler > create( boost::asio::io_context& ctx );

    boost::asio::ip::tcp::socket& socket();

    void start();

private:
    void readPacket( std::optional< resp::Request > inProgress = std::nullopt );

    boost::asio::ip::tcp::socket socket_;
    std::vector< char > data_;
};

} //namespace network
