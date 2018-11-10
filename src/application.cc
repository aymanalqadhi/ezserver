#include "application.h"

#include <net/async_tcp_client.h>

#include <boost/asio/io_context.hpp>
#include <termcolor/termcolor.hpp>

#include <iostream>
#include <thread>
#include <chrono>

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
    listener_->OnConnectionAccepted += [&](auto l, auto c) { NewClientsHandler(l,c); };
    return true;
}

// ======================================================== //

/**
 * Executed when a new valid connection is accepted
 *
 * @param listener  The listener that accepted the connection
 * @param socket    The accepted client socket
 */
void ezserver::Application::NewClientsHandler(
    const std::shared_ptr<ezserver::shared::net::ITcpListener>& listener,
    std::shared_ptr<boost::asio::ip::tcp::socket> socket)
{
    // Log an information message showing the
    // endpoint of the accepted connection
    LOG(logger_, Information) << "Got Connection From: " << socket->remote_endpoint() << std::endl;

    // Start a new acceptance job
    if (!listener->AcceptNext())
        throw std::runtime_error("Could not start the listener!");
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
}