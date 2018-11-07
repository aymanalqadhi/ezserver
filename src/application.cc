#include "application.h"

#include <boost/asio/io_context.hpp>
#include <termcolor/termcolor.hpp>
#include <iostream>

bool ezserver::Application::Startup()
{
    // Show welcome message
    std::cout << std::endl << termcolor::cyan << "\t-<( Welcome to EZ-SERVER! )>-" << std::endl << std::endl;

    // Initialize the thread pool
    LOG(logger_.lock(), Debug) << "Initializing thread pool with " << threads_count_ << " threads..." << std::endl;
    boost::asio::io_context::work w(io_);

    for (std::size_t i = 0; i < threads_count_; ++i)
    {
        thread_pool_.push_back(std::async(std::launch::async, [this] {
            this->io_.run();
        }));
    }

    if (!listener_->Initialize() || !listener_->Start())
    {
        LOG(logger_.lock(), Error) << "Could not start the TCP listener!" << std::endl;
        return false;
    }

    // Subscribe to new connections acceptance event
    listener_->OnConnectionAccepted += [&](auto l, auto c) { NewClientsHandler(l,c); };
    boost::asio::io_context::work work(io_);

    return true;
}

void ezserver::Application::NewClientsHandler(
    const std::shared_ptr<ezserver::shared::net::ITcpListener>& listener,
    std::shared_ptr<boost::asio::ip::tcp::socket>& client)
{
    LOG(logger_.lock(), Information) << "Got Connection From: " << client->remote_endpoint() << std::endl;

    if (!listener->AcceptNext())
        throw std::runtime_error("Could not start the listener!");
}