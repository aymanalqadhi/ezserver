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
        AsyncTcpClient(std::unique_ptr<boost::asio::ip::tcp::socket>&& client_socket)
            : client_socket_(client_socket)
        {
            std::cout << "Created socker " << client_socket->remote_endpoint() << std::endl;
        }

    private:
        std::unique_ptr<boost::asio::ip::tcp::socket>& client_socket_;
    };
}

#endif //EZSERVER_ASYNC_TCP_CLIENT_H
