#ifndef EZSERVER_APPLICATION_H
#define EZSERVER_APPLICATION_H

#include "iapplication.h"

#include <config/named_config.h>
#include <services/ilogger.h>
#include <net/itcp_listener.h>
#include <net/itcp_client.h>

#include <boost/di.hpp>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <future>

namespace ezserver
{
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
        : io_(io), logger_(logger), listener_(listener), threads_count_(threads_count) {}

        // Destructor
        virtual ~Application();

    private:

        /// The application io access service
        boost::asio::io_context& io_;

        /// A Service to manage app logs
        std::shared_ptr<ezserver::shared::services::ILogger> logger_;

        /// The application's main tcp listener
        std::shared_ptr<ezserver::shared::net::ITcpListener> listener_;

        /// The currently connected clients
        std::unordered_map<std::uint64_t, std::weak_ptr<ezserver::shared::net::ITcpClient>> clients_;

        //region Thread Pool

        // The user selected threads count
        const std::size_t& threads_count_;

        // The thread pool threads container
        std::vector<std::future<void>> thread_pool_;

        //endregion

        //region Private Methods

        /// Exectued upon new connection acceptance
        void NewClientsHandler(
            const std::shared_ptr<ezserver::shared::net::ITcpListener>& listener,
            std::shared_ptr<boost::asio::ip::tcp::socket> socket
        );

        /// Exectued upon a connection close
        void ConnectionClosed(
            const std::shared_ptr<ezserver::shared::net::ITcpClient>& client,
            const boost::system::error_code& err
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
