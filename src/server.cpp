// Created by Josh Goundry on 27/03/25

#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/buffers_iterator.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/impl/read_until.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/address_v4.hpp>
#include <boost/asio/streambuf.hpp>

std::string read( boost::asio::ip::tcp::socket& socket )
{
    boost::asio::streambuf buf;
    boost::asio::read_until( socket, buf, "\n" ); // async read until?

    return std::string( boost::asio::buffers_begin( buf.data() ), boost::asio::buffers_end( buf.data() ) );
}

void write( boost::asio::ip::tcp::socket& socket, const std::string& msg )
{
    boost::asio::write( socket, boost::asio::buffer( msg + "\n" ) );
}

int main( int argc, char* argv[] )
{
    std::cout << "Starting server...\n";

    boost::asio::io_context ctx;

    boost::asio::ip::tcp::acceptor acceptor( ctx, boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(), 6379 ) );

    boost::asio::ip::tcp::socket socket( ctx );

    acceptor.accept( socket );

    std::string msg = read( socket );

    std::cout << "Read message from client:\n";
    std::cout << msg << '\n';

    write( socket, "Hello from server!" );

    return 0;
}
