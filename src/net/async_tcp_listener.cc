#include "net/async_tcp_listener.h"

#include <net/async_tcp_client.h>
#include <net/client_accepted_eventargs.h>
#include <boost/asio/placeholders.hpp>

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
    auto args =
    acceptor_->async_accept(
        socket,
        std::bind(
            ezserver::shared::utils::EventHandler
                <const std::shared_ptr<ITcpListener>&,
                 std::shared_ptr<ezserver::shared::net::ITcpClient>&,
                 const boost::system::error_code&>::Invoke,
            this->OnConnectionAccepted,
            shared_from_this(),
            socket,
            boost::asio::placeholders::error()
        )
    );
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