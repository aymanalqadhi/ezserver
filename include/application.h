#ifndef EZSERVER_APPLICATION_H
#define EZSERVER_APPLICATION_H

#include "iapplication.h"

#include <config/named_config.h>
#include <services/ilogger.h>
#include <net/itcp_listener.h>
#include <net/itcp_client.h>

#include <boost/di.hpp>

#include <vector>
#include <map>
#include <future>
#include <regex>

namespace ezserver
{
    const std::string kRequestPattern = R"(^((?:\/[\w\-\.]+)+):(\w+)\s+(.*?)\n?$)";

    /**
     * Main application class
     */
    class Application : public ezserver::shared::IApplication
    {
    public:
        /// Base class method override
        virtual bool Startup() override;

        /**
         * Default DI constrcutor
         */
        BOOST_DI_INJECT(Application,
            boost::asio::io_context& io,
            const std::shared_ptr<ezserver::shared::services::ILogger>& logger,
            const std::shared_ptr<ezserver::shared::net::ITcpListener>& listener,
            (named = ezserver::config::named::ThreadsCount) const std::size_t& threads_count)
        : io_(io), logger_(logger), listener_(listener), threads_count_(threads_count),
          request_pattern_(kRequestPattern) {}

        // Destructor
        virtual ~Application();

        /**
         * Gets a reference to the plugins map
         * @return A reference to the plugins map
         */
        std::map<ezserver::shared::introp::PluginInfo, std::unique_ptr<ezserver::shared::introp::IPlugin>>& Plugins()
            override
        { return std::ref(plugins_); }

        /**
         * Gets the currently connected clients list
         * @return The clients list
         */
        virtual std::map<std::uint64_t, std::weak_ptr<ezserver::shared::net::ITcpClient>>& Clients() override
        { return std::ref(clients_); }

    private:

        /// The application io access service
        boost::asio::io_context& io_;

        /// A Service to manage app logs
        std::shared_ptr<ezserver::shared::services::ILogger> logger_;

        /// The application's main tcp listener
        std::shared_ptr<ezserver::shared::net::ITcpListener> listener_;

        /// The currently connected clients
        std::map<std::uint64_t, std::weak_ptr<ezserver::shared::net::ITcpClient>> clients_;

        /// The plugins imported by the application
        std::map<ezserver::shared::introp::PluginInfo, std::unique_ptr<ezserver::shared::introp::IPlugin>> plugins_;

        //region Thread Pool

        // The user selected threads count
        const std::size_t& threads_count_;

        // The thread pool threads container
        std::vector<std::future<void>> thread_pool_;

        //endregion

        //region Patterns

        /// The regular expression pattern to be matched
        /// with requests strings
        std::regex request_pattern_;

        //endregion

        //region Private Methods

        /// Exectued upon new connection acceptance
        void OnNewClientAccepted(
            const std::shared_ptr<ezserver::shared::net::ITcpListener> &listener,
            std::shared_ptr<boost::asio::ip::tcp::socket> socket
        );

        /// Exectued upon a connection close
        void OnConnectionClosed(
            const std::shared_ptr<ezserver::shared::net::ITcpClient> &client,
            const boost::system::error_code &err
        );

        /// Exectued upon a new message receiving
        void OnMessageReceived(
            const std::shared_ptr<ezserver::shared::net::ITcpClient> &client,
            std::string message
        );

        //endregion
    };

    /**
     * A Lambda to export this module
     */
    const auto application_module = [] {
        return boost::di::make_injector(
            boost::di::bind<ezserver::shared::IApplication>.to<Application>()
        );
    };
}

#endif //EZSERVER_APPLICATION_H
