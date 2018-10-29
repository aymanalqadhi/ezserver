#ifndef EZSERVER_CONFIGURATION_HPP
#define EZSERVER_CONFIGURATION_HPP

#include <boost/program_options.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

/// Configuration Namespace
namespace ezserver::config
{
    /**
     * Gets the general options object
     * @return The general options object
     */
    boost::program_options::options_description GetGeneralOptions() {
        boost::program_options::options_description general_options("Global Options");
        general_options.add_options()
            ("version,v", "Displays the version of the app")
            ("help,h", "Displays a help message");
        return general_options;
    }

    /**
     * Gets the connection options object
     * @return The connection options object
     */
    boost::program_options::options_description GetConnectionOptions() {
        boost::program_options::options_description connection_options("Connection Options");
        connection_options.add_options()
                ("no-ssl", "Disables ssl support")
                ("max-connections", boost::program_options::value<std::size_t>()->default_value(1000), "Sets the maximum number of connection at the same time");
        return connection_options;
    }

    /**
     * Gets the logging options object
     * @return The logging options object
     */
    boost::program_options::options_description GetLoggingOptions() {
        boost::program_options::options_description logging_options("Logging Options");
        logging_options.add_options()
                ("log,l", "Sets the logging level value");

        return logging_options;
    }

    /**
     * Gets the plugins options object
     * @return The plugins options object
     */
    boost::program_options::options_description GetPluginsOptions() {
        boost::program_options::options_description plugins_options("Plugins Options");
        plugins_options.add_options()
                ("plugins,pp", boost::program_options::value<std::string>()->default_value("plugins"), "Sets the path of the application's plugins");

        return plugins_options;
    }

    /**
     * Gets all program options
     * @return The options object
     */
    std::shared_ptr<boost::program_options::options_description> GetOptionsAggrgator() {
        auto aggregator = std::make_shared<boost::program_options::options_description>("All Options");
        auto general = GetGeneralOptions();

        aggregator->add(GetGeneralOptions());
        aggregator->add(GetConnectionOptions());
        aggregator->add(GetLoggingOptions());
        aggregator->add(GetPluginsOptions());

        return aggregator;
    }
}

#endif //EZSERVER_CONFIGURATION_HPP
