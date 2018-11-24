#include "application.h"

#include <net/async_tcp_client.h>

#include <boost/asio/io_context.hpp>
#include <termcolor/termcolor.hpp>

#include <iostream>
#include <string_view>

// ======================================================== //

bool ezserver::Application::Startup()
{
    // Show welcome message
    std::cout << std::endl << termcolor::cyan << "\t-<( Welcome to EZ-SERVER! )>-" << std::endl << std::endl;

    // Initialize the thread pool
    LOG(logger_, Debug) << "Initializing thread pool with " << threads_count_ << " threads..." << std::endl;
    boost::asio::io_context::work w(io_);

    for (std::size_t i = 0; i < threads_count_; ++i)
    {
        // Add a new asynchronous job to the pool
        thread_pool_.push_back(std::async(std::launch::async, [&] { this->io_.run(); }));
    }

    // Initialize & Start the listener
    if (!listener_->Initialize() || !listener_->Start())
    {
        LOG(logger_, Error) << "Could not start the TCP listener!" << std::endl;
        return false;
    }

    // Subscribe to new connections acceptance event
    listener_->NewConnectionAccepted += std::bind(
            &ezserver::Application::OnNewClientAccepted, this,
            std::placeholders::_1,
            std::placeholders::_2
    );

    return true;
}

// ======================================================== //

/**
 * Executed when a new valid connection is accepted
 *
 * @param listener  The listener that accepted the connection
 * @param scket    The accepted client socket
 */
void ezserver::Application::OnNewClientAccepted(
        const std::shared_ptr<ezserver::shared::net::ITcpListener> &listener,
        std::shared_ptr<boost::asio::ip::tcp::socket> socket)
{
    // Global atomic clients counter
    static std::atomic_uint64_t clients_counter(0);

    // Log an information message showing the
    // endpoint of the accepted connection
    LOG(logger_, Information) << "Got Connection From: " << socket->remote_endpoint() << std::endl;

    // Create a client object out of the accepted socket
    std::shared_ptr<ezserver::shared::net::ITcpClient> client(new ezserver::net::AsyncTcpClient(std::move(socket)));
    client->Id(++clients_counter);

    // Subscribe to event handlers
    client->ConnectionClosed += std::bind(
        &ezserver::Application::OnConnectionClosed, this,
        std::placeholders::_1,
        std::placeholders::_2
    );
    client->MessageRecieved += std::bind(
        &ezserver::Application::OnMessageReceived, this,
        std::placeholders::_1,
        std::placeholders::_2
    );

    // Start the client
    client->Start();

    // Add the current time stamp & the created client to the clients map
    clients_.emplace(std::make_pair(client->Id(), std::weak_ptr<ezserver::shared::net::ITcpClient>(client)));

    // Start a new acceptance job
    if (!listener->AcceptNext())
        throw std::runtime_error("Could not start the listener!");
}

// ======================================================== //

void ezserver::Application::OnConnectionClosed(
        const std::shared_ptr<ezserver::shared::net::ITcpClient> &client,
        const boost::system::error_code &err)
{
    // Remove the corrosponding weak_ptr from the clients map
    clients_.erase(client->Id());

    if (client->Socket()->is_open())
    {
        if (err == boost::asio::error::eof)
        {
            LOG(logger_, Information)
                    << "Connection to " << client->Socket()->remote_endpoint()
                    << " was closed." << std::endl;
        }
        else
        {
            LOG(logger_, Warning)
                    << "Connection to " << client->Socket()->remote_endpoint()
                    << "was unexpectedly closed" << err.message() << "!" << std::endl;
        }
        client->Stop();
    }
    else
    {
        LOG(logger_, Information) << "Connection to client #" << client->Id() << " was closed." << std::endl;
    }
}

// ======================================================== //

void ezserver::Application::OnMessageReceived(
    const std::shared_ptr<ezserver::shared::net::ITcpClient> &client,
    std::string message)
{
    std::smatch matches;
    if (!std::regex_search(message, matches, request_pattern_))
    {
        LOG(logger_, Error) << "Invalid Command `" << message << "` Received From: "
                            << client->Socket()->remote_endpoint() << std::endl;
        client->Socket()->send(boost::asio::buffer("!Invalid Command `" + std::move(message) + "`!\n"));
        return;
    }

    auto commands_in_path = commands_.get<ezserver::shared::path>().find(matches[1].str());
    if (commands_in_path == commands_.get<ezserver::shared::path>().end())
    {

    }
//    command(client, "Ayman");
}

// ======================================================== //

/**
 * The application destructor
 * Contains the cleanup logic of
 * the application
 */
ezserver::Application::~Application()
{
    // Clear all threads
    thread_pool_.clear();

    // Unload loaded plugins
    for (const auto &plugin : plugins_)
    {
        LOG(logger_, Debug) << "Unloading `" << plugin.first.Name() << "`..." << std::endl;

        plugin.second->Lib().unload();
        plugins_.erase(plugin.first);
    }
}