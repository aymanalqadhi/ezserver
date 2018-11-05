#ifndef EZSERVER_ASYNC_TCP_LISTENER_H
#define EZSERVER_ASYNC_TCP_LISTENER_H

#include "net/itcp_listener.h"

#include <services/ilogger.h>
#include <config/named_config.h>
#include <boost/di.hpp>

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

        /**
         * Default DI constructor
         */
        BOOST_DI_INJECT(AsyncTcpListener,
            const std::shared_ptr<ezserver::shared::services::ILogger>& logger,
            (named = ezserver::config::named::Port) const unsigned short& port,
            (named = ezserver::config::named::Backlog) const int&backlog
        ) : logger_(logger), port_(port), backlog_(backlog) {}

    private:

        /// A Service to manage app logs
        std::weak_ptr<ezserver::shared::services::ILogger> logger_;

        /// The port to listen on
        const unsigned short& port_;

        /// The listener backlog
        const int& backlog_;
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
