#include "net/async_tcp_listener.h"

#include <net/async_tcp_client.h>
#include <net/client_accepted_eventargs.h>

#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>


// ==================================================== //

bool ezserver::net::AsyncTcpListener::Initialize()
{
    // Logs a debug message showing that the listener
    // is being initialized
    LOG(logger_, Debug) << "Creating & initializing TCP listener... " << std::endl;

    // Create a low level acceptor
    acceptor_ = std::make_shared<boost::asio::ip::tcp::acceptor>(io_);

    return true;
}


// ==================================================== //

bool ezserver::net::AsyncTcpListener::Start()
{
    // Return if the listener was already started
    if (IsStarted())
    {
        // Log a debug message before exiting
        LOG(logger_, Debug) << "Listener is already started!" << std::endl;
        return true;
    }

    // Logs an information message showing the port & the backtrace
    // of the listener
    LOG(logger_, Information)
            << "Starting listener on port " << port_
            << ", with a backlog of " << backlog_ << std::endl;

    try
    {
        /*          Open the listener       */
        if (!acceptor_->is_open())
            acceptor_->open(boost::asio::ip::tcp::v4());

        /*          Bind & Start the listener       */
        boost::asio::ip::tcp::endpoint listen_on(boost::asio::ip::tcp::v4(), port_);
        acceptor_->bind(listen_on);
        acceptor_->listen(backlog_);

        // Log a debug message showing the address
        // and port that the listener is listening on
        LOG(logger_, Debug) << "Listening On: " << listen_on << std::endl;

        // Update the starting status value
        is_started_.store(true);

        // Accept a new connection
        return AcceptNext();
    }
    catch(const std::exception& ex)
    {
        // Log the error message, and exit
        LOG(logger_, Error) << ex.what() << std::endl;
        return false;
    }
}

// ==================================================== //

/// TODO: Implement this
bool ezserver::net::AsyncTcpListener::Stop(bool force)
{
    return false;
}

// ==================================================== //

bool ezserver::net::AsyncTcpListener::AcceptNext()
{
    // Throw an exception if the listener is
    // not started yet
    if (!IsStarted())
        throw std::runtime_error("Cannot accept connections, Listener is not started!");

    // Create a new socket
    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(io_);

    // Accept a connection asynchronously, and then
    // store it in the `socket` variable
    acceptor_->async_accept(
        *socket,
        boost::bind(&AsyncTcpListener::HandleAcceptedConnections, this, socket, boost::asio::placeholders::error)
    );

    return true;
}

// =================================================== //

void ezserver::net::AsyncTcpListener::HandleAcceptedConnections(
    std::shared_ptr<boost::asio::ip::tcp::socket> socket,
    const boost::system::error_code& error)
{
    // If there was an error, log the error
    // message and exit
    if (error)
    {
        LOG(logger_, Error) << error.message() << std::endl;
        return;
    }

    // Invoke the associated event handlers with
    // a pointer to the current listener & the
    // accepted socket
    ezserver::shared::net::ITcpListener::NewConnectionAccepted.Invoke(
        shared_from_this(),
        socket
    );
}