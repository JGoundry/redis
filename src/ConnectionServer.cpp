// Created by Josh Goundry on 29/03/25

#include "ConnectionServer.hpp"
#include <boost/asio/placeholders.hpp>

ConnectionServer::ConnectionServer( const int port ) : acceptor_( ctx_, boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(), port ) )
{
}

ConnectionServer::~ConnectionServer()
{
    if ( !ctx_.stopped() )
    {
        ctx_.stop();
    }

    acceptor_.close();
}

// pass in function pointer to handle request
void ConnectionServer::StartAsync()
{
    StartAccept();
    ctx_.run();
}

void ConnectionServer::StartAccept()
{

    std::shared_ptr< Connection > connection = std::make_shared< Connection >( ctx_ );
    acceptor_.async_accept( connection->socket(), std::bind( &ConnectionServer::HandleAccept, this, connection, boost::asio::placeholders::error ) );
}

void ConnectionServer::HandleAccept( const std::shared_ptr< Connection >& connection, const boost::system::error_code& ec )
{
    if ( !ec )
    {
        HandleRequest( connection );
    }
    else
    {
        std::cerr << "Accept error: " << ec.message() << std::endl;
    }

    StartAccept();
}

void ConnectionServer::HandleRequest( const std::shared_ptr< Connection >& connection )
{
    connection->start();
}
