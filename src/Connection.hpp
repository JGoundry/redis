// Created by Josh Goundry on 29/03/25

#include <memory>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>

class Connection : public std::enable_shared_from_this< Connection >
{
    struct Private{ explicit Private() = default; };

public:
    Connection( const Private, boost::asio::io_context& ctx );

    static std::shared_ptr< Connection > create( boost::asio::io_context& ctx );

    boost::asio::ip::tcp::socket& socket();

    void start();

private:
    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf streambuf_;
};
