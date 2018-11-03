#ifndef EZSERVER_ITCP_CLIENT_H
#define EZSERVER_ITCP_CLIENT_H

#include <boost/asio/io_context.hpp>
#include <memory>

namespace ezserver::shared::net
{
    class ITcpClient : public std::enable_shared_from_this<ITcpClient>
    {

    };
}

#endif //EZSERVER_ITCP_CLIENT_H
