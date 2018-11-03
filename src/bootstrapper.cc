#include "bootstrapper.h"

#include <utils/time.h>
#include <termcolor/termcolor.hpp>

//===========================================================================//

bool ezserver::Bootstrapper::Bootstrap()
{
    // Start by initializing the application core services
    std::cout << termcolor::bold << termcolor::cyan << "[+] Initializing Services..." << termcolor::reset << std::endl;
    if (!LoadServices())
        throw std::runtime_error("Could not load services!");

    // Load plugins
    LOG(logger_.lock(), Information) << "Loading Plugins..." << std::endl;
    if (!LoadPlugins())
        throw std::runtime_error("Could not load plugins!");

    /// TODO:
    /// * Load services provieded by plugins

    return true;
}

//===========================================================================//

bool ezserver::Bootstrapper::LoadPlugins()
{
    // Try to load the plugins. And if there was
    // an error, just abort the task

    try
    {
        // Load plugins
        auto plugins = plugins_loader_.lock()->LoadPlugins();

        // Initialize plugins
        LOG(logger_.lock(), Information) << "Initializing Plugins..." << std::endl;
    }
    catch (const std::exception& ex)
    {
        LOG(logger_.lock(), Trace) << ex.what();
        return false;
    }

    return true;
}

//===========================================================================//

bool ezserver::Bootstrapper::LoadServices()
{
    // Load every service registered in the service manager

    for (const auto &svc : this->services_manager_.lock()->Services())
    {
        // Log the name of the current service to be initialized
        // using the standard way because we don't have the logging
        // service ready yet (or it might be initialized yet).

        std::cout << termcolor::bold << termcolor::cyan
                  << ezserver::utils::time::GetTimeString(std::time(0), "[%H:%M:%S] ") << termcolor::reset
                  << "Initializing Service \"" << svc.second->Name() << "\"... " << std::flush;


        // Try to initialize the service. And if failed just abort the whole operation
        // if the service was required by the app, otherwise, just show a warning message
        // and continue the application execution

        if (svc.second->Initialize())
            std::cout << termcolor::bold << termcolor::green << "\r[  Done  ]" << termcolor::reset << std::endl;
        else
        {
            std::cout << termcolor::bold << termcolor::red << "\r[  Fail  ]" << termcolor::reset << std::endl;
            if (svc.second->IsRequired())
            {
                std::cout << termcolor::bold << termcolor::red
                          << ezserver::utils::time::GetTimeString(std::time(0), "[%H:%M:%S] ") << termcolor::reset
                          << "Could not initialize required service: " << svc.second->Name() << "!" << std::flush;

                return false;
            }
            else
            {
                std::cout << termcolor::bold << termcolor::yellow
                          << ezserver::utils::time::GetTimeString(std::time(0), "[%H:%M:%S] ") << termcolor::reset
                          << "Could not initialize service: " << svc.second->Name() << "!" << std::flush;
            }
        }
    }

    return true;
}

//===========================================================================//

bool ezserver::Bootstrapper::Run()
{
    // Bootstrap app main pieces
    this->Bootstrap();

    // Notify completion
    LOG(logger_.lock(), Information) << "Bootstrapping Completed!" << std::endl;

    // Startup the application
    return this->application_.lock()->Startup();
}