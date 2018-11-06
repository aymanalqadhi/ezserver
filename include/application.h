#ifndef EZSERVER_APPLICATION_H
#define EZSERVER_APPLICATION_H

#include "iapplication.h"

#include <services/ilogger.h>
#include <net/itcp_listener.h>

#include <boost/di.hpp>
#include <boost/thread/thread.hpp>
#include <config/named_config.h>

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
            const std::shared_ptr<ezserver::shared::services::ILogger>& logger,
            boost::asio::io_context& io,
            const std::shared_ptr<ezserver::shared::net::ITcpListener>& listener,
            (named = ezserver::config::named::ThreadsCount) const std::size_t& threads_count)
        : logger_(logger), io_(io), listener_(listener), thread_pool_(io), threads_count_(threads_count) {}

    private:
        /// A Service to manage app logs
        std::weak_ptr<ezserver::shared::services::ILogger> logger_;

        /// The application io access service
        boost::asio::io_context& io_;

        /// The application's main tcp listener
        std::shared_ptr<ezserver::shared::net::ITcpListener> listener_;

        /// The application thread pool
        boost::thread_group thread_pool_;

        /// The threads pool threads count
        const std::size_t& threads_count_;

        /*      Private Mehods      */
        void NewClientsHandler(
            const std::shared_ptr<ezserver::shared::net::ITcpListener>& listener,
            std::shared_ptr<boost::asio::ip::tcp::socket>& client
        );
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
