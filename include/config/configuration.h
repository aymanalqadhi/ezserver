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
    boost::program_options::options_description GetGeneralOptions();

    /**
     * Gets the connection options object
     * @return The connection options object
     */
    boost::program_options::options_description GetConnectionOptions();

    /**
     * Gets the logging options object
     * @return The logging options object
     */
    boost::program_options::options_description GetLoggingOptions();

    /**
     * Gets the plugins options object
     * @return The plugins options object
     */
    boost::program_options::options_description GetPluginsOptions();

    /**
     * Gets all program options
     * @return The options object
     */
    std::shared_ptr<boost::program_options::options_description> GetOptionsAggrgator();
}

#endif //EZSERVER_CONFIGURATION_HPP
