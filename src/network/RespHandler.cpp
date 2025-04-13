// Created by Josh Goundry on 30/03/25

#include "RespHandler.hpp"

#include "resp/parseRespRequest.hpp"

#include <boost/asio/buffer.hpp>
#include <boost/asio/buffers_iterator.hpp>
#include <boost/asio/completion_condition.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/read.hpp>

#include <cctype>
#include <cstddef>
#include <iostream>
#include <optional>

namespace network
{

RespHandler::RespHandler( const Private, boost::asio::io_context& ctx ) : socket_( ctx )
{
    data_.reserve( 1024 );
}

std::shared_ptr< RespHandler > RespHandler::create( boost::asio::io_context& ctx )
{
    return std::make_shared< RespHandler >( Private(), ctx );
}

boost::asio::ip::tcp::socket& RespHandler::socket()
{
    return socket_;
}

// Implements RESP2 protocol
void RespHandler::start()
{
    readPacket();
}

void RespHandler::readPacket( std::optional< resp::Request > inProgress )
{
    boost::asio::async_read( socket_, boost::asio::dynamic_buffer( data_, 1024 ), boost::asio::transfer_at_least( 1 ), [self = shared_from_this()]( const boost::system::error_code& ec, std::size_t bytes_transferred ) {
        if ( ec )
        {
            std::cerr << "Error: " << ec.message() << std::endl;
            return;
        }

        resp::Request request;

        const bool success = resp::parseRespRequest( self->data_, request );
        self->data_.clear();

        std::cout << request << '\n';

        if ( success )
        {
            std::cout << "Successfully parsed packet\n";
            // handle request
        }
        else
        {
            std::cout << "Failed parsing packet\n";
        }

        // continue reading
        self->readPacket();
    } );
}

} //namespace network
