#ifndef EZSERVER_ASYNC_TCP_CLIENT_H
#define EZSERVER_ASYNC_TCP_CLIENT_H

#include "net/itcp_client.h"
#include <boost/asio/ip/tcp.hpp>
#include <iostream>

namespace ezserver::net
{
    class AsyncTcpClient : public ezserver::shared::net::ITcpClient
    {
    public:

        /**
         * Default constructor
         * @param client_socket The socket to use
         */
        AsyncTcpClient(std::shared_ptr<boost::asio::ip::tcp::socket> client_socket)
            : client_socket_(client_socket), io_(client_socket->get_io_context()) {}

        /**
         * Alternative constructor to construct a new socket
         * using a given io_context
         * @param io The io_context to use
         */
        AsyncTcpClient(boost::asio::io_context& io)
            : AsyncTcpClient(std::make_shared<boost::asio::ip::tcp::socket>(io)) {}

        /**
         * The the low level socket
         * @return a pointer to the inner socket
         */
        std::shared_ptr<boost::asio::ip::tcp::socket>& Socket() { return client_socket_; }

    private:
        /// The inner socket used by this wrapper
        std::shared_ptr<boost::asio::ip::tcp::socket>& client_socket_;

        /// The io_context to get system access
        boost::asio::io_context& io_;
    };
}

#endif //EZSERVER_ASYNC_TCP_CLIENT_H
