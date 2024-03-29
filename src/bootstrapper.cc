#include "bootstrapper.h"

#include <utils/time.h>
#include <termcolor/termcolor.hpp>

#include <string_view>
#include <map>

//===========================================================================//

void ezserver::Bootstrapper::Bootstrap()
{
    // Start by initializing the application core services
    // and throw a fatal exception in case of failure
    std::cout << termcolor::bold << termcolor::cyan << "[+] Bootstrapping Services..." << termcolor::reset << std::endl;
    if (!LoadServices())
        throw std::runtime_error("Could not load services!");

    // Load plugins. and throw a
    // fatal exception in case of failure
    LOG(logger_, Information) << "Loading Plugins..." << std::endl;
    if (!LoadPlugins())
        throw std::runtime_error("Could not load plugins!");
}

//===========================================================================//

bool ezserver::Bootstrapper::LoadPlugins()
{
    // Try to load the plugins. And if there was
    // an error, just abort the task

    try
    {
        // Load plugins
        LOG(logger_, Debug) << "Loading Plugins..." << std::endl;
        plugins_loader_->LoadPlugins(application_->Plugins());

        // Return if there were no plugins
        if (application_->Plugins().empty()) return true;

        // Initialize plugins
        LOG(logger_, Debug) << "Initializing Plugins..." << std::endl;
        plugins_loader_->InitializePlugins(application_->Plugins(), services_manager_);

        // Load commands
        LOG(logger_, Debug) << "Loading Commands..." << std::endl;
        plugins_loader_->LoadCommands(application_->Plugins(), application_->Commands());
    }
    catch (const std::exception& ex)
    {
        // log the exception message, and exit
        LOG(logger_, Trace) << ex.what() << std::endl;
        return false;
    }

    return true;
}

//===========================================================================//

bool ezserver::Bootstrapper::LoadServices()
{
    // Load every service registered in the service manager

    for (const auto &svc : this->services_manager_->Services())
    {
        // Log the name of the current service to be initialized
        // using the standard way because we don't have the logging
        // service ready yet (or it might be initialized yet).

        std::cout << termcolor::bold << termcolor::cyan
                  << ezserver::utils::time::GetTimeString(std::time(0), "[%H:%M:%S] ") << termcolor::reset
                  << "Bootstrapping Service: " << svc.second->Name() << "... " << std::endl;


        // Try to initialize the service. And if failed just abort the whole operation
        // if the service was required by the app, otherwise, just show a warning message
        // and continue the application execution

        if (svc.second->Bootstrap())
            std::cout << termcolor::bold << termcolor::green << "\r[  Done  ]"
                      << termcolor::reset << " Bootstrapped service: " << svc.second->Name() << "." << std::endl;
        else
        {
            std::cout << termcolor::bold << termcolor::red << "\r[  Fail  ]"
                      << termcolor::reset << " Could not bootstrap service: " <<  svc.second->Name() << "." << std::endl;

            if (svc.second->IsRequired())
            {
                std::cout << termcolor::bold << termcolor::red
                          << ezserver::utils::time::GetTimeString(std::time(0), "[%H:%M:%S] ") << termcolor::reset
                          << "Could not bootstrap required service: " << svc.second->Name() << "!" << std::flush;

                return false;
            }
            else
            {
                std::cout << termcolor::bold << termcolor::yellow
                          << ezserver::utils::time::GetTimeString(std::time(0), "[%H:%M:%S] ") << termcolor::reset
                          << "Could not bootstrap service: " << svc.second->Name() << "!" << std::flush;
            }
        }
    }

    return true;
}

//===========================================================================//

void ezserver::Bootstrapper::Run()
{
    // Bootstrap app main pieces
    this->Bootstrap();

    // Notify completion
    LOG(logger_, Information) << "Bootstrapping Completed!" << std::endl;

    // Startup the application
    if (!this->application_->Startup())
        throw std::runtime_error("Could not startup application!");
}
