// Created by Josh Goundry on 30/03/25

#include "Connection.hpp"

#include <boost/asio/placeholders.hpp>
#include <boost/asio/read_until.hpp>

#include <iostream>

Connection::Connection( boost::asio::io_context& ctx ) : socket_( ctx )
{
}

boost::asio::ip::tcp::socket& Connection::socket()
{
    return socket_;
}

void Connection::start()
{
    boost::asio::async_read_until( socket_, streambuf_, "\n", [self = shared_from_this()]( const boost::system::error_code& ec, std::size_t bytes_transferred ) {
        if ( !ec )
        {
            std::istream is( &self->streambuf_ );
            std::string line;
            std::getline( is, line );
            std::cout << "Received line: " << line << std::endl;
            self->start(); // Continue reading
        }
        else if ( ec != boost::asio::error::eof )
        {
            std::cerr << "Error: " << ec.message() << std::endl;
        }
    } );
}
