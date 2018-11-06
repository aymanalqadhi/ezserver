#ifndef EZSERVER_ITCP_LISTENER_H
#define EZSERVER_ITCP_LISTENER_H

#include <net/itcp_client.h>
#include <utils/event_handler.h>

#include <boost/asio/ip/tcp.hpp>
#include <memory>

/// Shared Net Namespace
namespace ezserver::shared::net
{
    /**
     * The base class of TCP listeners
     */
    class ITcpListener : public std::enable_shared_from_this<ITcpListener>
    {
    public:

        /**
         * Initializes the listener
         * @return The operation result
         */
        virtual bool Initialize() = 0;

        /**
         * Starts the listener
         * @param backlog The maximum number of connection requests
         * @param port    The port to listen on
         * @return The operation result
         */
        virtual bool Start() = 0;

        /**
         * Stops the listener
         * @param force If true, the listener is forced to shutdown
         * @return The operation result
         */
        virtual bool Stop(bool force) = 0;

        /// Listens for a new connection
        virtual bool AcceptNext() = 0;

        /**
         * Gets whether the listener is started or not
         * @return The listener listening status
         */
        virtual inline const bool IsStarted() const noexcept = 0;

        /// An event handler to be invoked when a new connection is accpeted
        ezserver::shared::utils::EventHandler<const std::shared_ptr<ITcpListener>&,
            std::shared_ptr<boost::asio::ip::tcp::socket>&> OnConnectionAccepted;
    };
}

#endif //EZSERVER_ITCP_LISTENER_H
