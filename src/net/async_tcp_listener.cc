#include "net/async_tcp_listener.h"

#include <net/async_tcp_client.h>
#include <net/client_accepted_eventargs.h>

#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>

// ==================================================== //

bool ezserver::net::AsyncTcpListener::Start()
{
    try
    {
        /*          Open the listener       */
        if (!acceptor_->is_open())
            acceptor_->open(boost::asio::ip::tcp::v4());

        /*          Bind & Start the listener       */
        boost::asio::ip::tcp::endpoint listen_on(boost::asio::ip::tcp::v4(), port_);
        acceptor_->bind(listen_on);
        acceptor_->listen(backlog_);

        LOG(logger_.lock(), Debug) << "Listening On: " << listen_on << std::endl;

        return AcceptNext();
    }
    catch(const std::exception& ex)
    {
        LOG(logger_.lock(), Error) << ex.what();
        return false;
    }
}

// ==================================================== //

bool ezserver::net::AsyncTcpListener::Stop(bool force)
{
    return false;
}

// ==================================================== //

bool ezserver::net::AsyncTcpListener::AcceptNext()
{
    if (!IsStarted()) return false;

    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(io_);
    acceptor_->async_accept(
        *socket,
        boost::bind(&AsyncTcpListener::HandleAcceptedConnections, this, socket, boost::asio::placeholders::error())
    );

    return true;
}

// ==================================================== //

bool ezserver::net::AsyncTcpListener::Initialize()
{
    LOG(logger_.lock(), Debug) << "Creating & initializing TCP listener... " << std::endl;
    acceptor_ = std::make_shared<boost::asio::ip::tcp::acceptor>(io_);

    LOG(logger_.lock(), Information)
        << "Starting listener on port " << port_
        << ", with a backlog of " << backlog_ << std::endl;

    return true;
}

// =================================================== //

void ezserver::net::AsyncTcpListener::HandleAcceptedConnections(
    std::shared_ptr<boost::asio::ip::tcp::socket>& socket,
    boost::system::error_code& error)
{
    if (error)
    {
        LOG(logger_.lock(), Error) << error.message() << std::endl;
        return;
    }

    ezserver::shared::net::ITcpListener::OnConnectionAccepted.Invoke(
        shared_from_this(),
        socket
    );
}