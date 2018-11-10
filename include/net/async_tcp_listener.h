#ifndef EZSERVER_ASYNC_TCP_LISTENER_H
#define EZSERVER_ASYNC_TCP_LISTENER_H

#include "net/itcp_listener.h"

#include <services/ilogger.h>
#include <config/named_config.h>

#include <boost/di.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <boost/system/error_code.hpp>

namespace ezserver::net
{
    /**
     * A TCP listener that uses asynchronous methods
     * in order to function
     */
    class AsyncTcpListener : public ezserver::shared::net::ITcpListener
    {
    public:
        /// Base class method override
        virtual bool Start() override;

        /// Base class method override
        virtual bool Stop(bool force) override;

        /// Base class method override
        virtual bool Initialize() override;

        /**
         * Default DI constructor
         */
        BOOST_DI_INJECT(AsyncTcpListener,
            const std::shared_ptr<ezserver::shared::services::ILogger>& logger,
            boost::asio::io_context& io,
            (named = ezserver::config::named::Port) const unsigned short& port,
            (named = ezserver::config::named::Backlog) const int&backlog
        ) : logger_(logger), io_(io), port_(port), backlog_(backlog), is_started_(false) {}

        /**
         * Gets whether the listener is started or not
         * @return The listener listening status
         */
        virtual inline const bool IsStarted() const noexcept override { return is_started_; }

    private:

        /// A Service to manage app logs
        std::shared_ptr<ezserver::shared::services::ILogger> logger_;

        /// The application main io intraction object
        boost::asio::io_context& io_;

        /// The port to listen on
        const unsigned short& port_;

        /// The listener backlog
        const int& backlog_;

        /// The low level tcp acceptor object
        std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor_;

        /// Determines of the listener is started or not
        std::atomic_bool is_started_;

        /*          Private Methods     */

        /// Listens for a new connection
        bool AcceptNext();

        /// Handles incoming connections
        void HandleAcceptedConnections(std::shared_ptr<boost::asio::ip::tcp::socket>, const boost::system::error_code&);
    };

    /**
     * A Lambda to export this module
     */
    auto async_tcp_listener_module = []{
        return boost::di::make_injector(
            boost::di::bind<ezserver::shared::net::ITcpListener>.to<AsyncTcpListener>()
        );
    };
}

#endif //EZSERVER_ASYNC_TCP_LISTENER_H
