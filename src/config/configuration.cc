#include "config/configuration.h"

// ======================================================= //

boost::program_options::options_description ezserver::config::GetGeneralOptions() {
    boost::program_options::options_description general_options("Global Options");
    general_options.add_options()
            (
                "threads,t",
                boost::program_options::value<std::size_t>()->default_value(64),
                "Sets the maximum number of threads in the thread pool"
            )
            ("version,v", "Displays the version of the app")
            ("help,h", "Displays a help message");
    return general_options;
}

// ======================================================= //

boost::program_options::options_description ezserver::config::GetConnectionOptions() {
    boost::program_options::options_description connection_options("Connection Options");
    connection_options.add_options()
            ("port", boost::program_options::value<unsigned short>()->default_value(3434), "The port to listen on")
            ("backlog", boost::program_options::value<int>()->default_value(1000), "Maximum number of connection requests at a time")
            ("no-ssl", "Disables ssl support")
            ("max-connections", boost::program_options::value<std::size_t>()->default_value(1000), "Sets the maximum number of connection at the same time");
    return connection_options;
}

// ======================================================= //

boost::program_options::options_description ezserver::config::GetLoggingOptions() {
    boost::program_options::options_description logging_options("Logging Options");
    logging_options.add_options()
            ("log,l", "Sets the logging level value");

    return logging_options;
}

// ======================================================= //

boost::program_options::options_description ezserver::config::GetPluginsOptions() {
    boost::program_options::options_description plugins_options("Plugins Options");
    plugins_options.add_options()
            ("plugins,pp", boost::program_options::value<std::string>()->default_value("plugins"), "Sets the path of the application's plugins");

    return plugins_options;
}

// ======================================================= //

std::shared_ptr<boost::program_options::options_description> ezserver::config::GetOptionsAggrgator() {
    auto aggregator = std::make_shared<boost::program_options::options_description>("All Options");
    auto general = GetGeneralOptions();

    aggregator->add(GetGeneralOptions());
    aggregator->add(GetConnectionOptions());
    aggregator->add(GetLoggingOptions());
    aggregator->add(GetPluginsOptions());

    return aggregator;
}