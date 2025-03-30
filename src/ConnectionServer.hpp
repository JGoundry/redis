// Created by Josh Goundry on 29/03/25

#include "Connection.hpp"

#include <iostream>
#include <memory>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

class ConnectionServer
{
public:
    explicit ConnectionServer( const int port );
    ~ConnectionServer();
    void StartAsync();

private:
    void StartAccept();
    void HandleAccept( const std::shared_ptr< Connection >& connection, const boost::system::error_code& ec );
    void HandleRequest( const std::shared_ptr< Connection >& connection );

private:
    boost::asio::io_context ctx_;
    boost::asio::ip::tcp::acceptor acceptor_;
};
