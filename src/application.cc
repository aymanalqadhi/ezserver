#include "application.h"

#include <termcolor/termcolor.hpp>
#include <iostream>

bool ezserver::Application::Startup()
{
    // Show welcome message
    std::cout << std::endl << termcolor::cyan << "\t-<( Welcome to EZ-SERVER! )>-" << std::endl << std::endl;

    // Start the listener
    if (!listener_->Initialize() || !listener_->Start())
    {
        LOG(logger_.lock(), Error) << "Could not start the TCP listener!" << std::endl;
        return false;
    }


    listener_->OnConnectionAccepted += [](auto listener, auto client, auto error)
    {

    };

    return true;
}