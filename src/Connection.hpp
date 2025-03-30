// Created by Josh Goundry on 29/03/25

#include <memory>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>

class Connection : public std::enable_shared_from_this< Connection >
{
public:
    explicit Connection( boost::asio::io_context& ctx );

    boost::asio::ip::tcp::socket& socket();

    void start();

private:
    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf streambuf_;
};
