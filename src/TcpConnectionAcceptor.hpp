// Created by Josh Goundry on 29/03/25

#include <iostream>
#include <memory>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>

template< typename TcpConnection >
class TcpConnectionAcceptor
{
public:
    explicit TcpConnectionAcceptor( const int port ) : acceptor_( ctx_, boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(), port ) )
    {
    }

    ~TcpConnectionAcceptor()
    {
        if ( !ctx_.stopped() )
        {
            ctx_.stop();
        }

        acceptor_.close();
    }

    void StartAsync()
    {
        StartAccept();
        ctx_.run();
    }

private:
    void StartAccept()
    {

        std::shared_ptr< TcpConnection > connection = TcpConnection::create( ctx_ );
        acceptor_.async_accept( connection->socket(), std::bind( &TcpConnectionAcceptor::HandleAccept, this, connection, boost::asio::placeholders::error ) );
    }
    void HandleAccept( const std::shared_ptr< TcpConnection >& connection, const boost::system::error_code& ec )
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
    void HandleRequest( const std::shared_ptr< TcpConnection >& connection )
    {
        connection->start();
    }

private:
    boost::asio::io_context ctx_;
    boost::asio::ip::tcp::acceptor acceptor_;
};
