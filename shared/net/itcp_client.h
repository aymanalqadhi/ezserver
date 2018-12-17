#ifndef EZSERVER_ITCP_CLIENT_H
#define EZSERVER_ITCP_CLIENT_H

#include <utils/event_handler.h>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>

#include <memory>

namespace ezserver::shared::net
{
    /**
     * An enum to represent response codes
     */
    enum class ResponseCode : std::uint8_t {
        kSuccess = 0x0, kFailure = 0x1, kInvalidRequest = 0x3,
        kCommandNotFound = 0x4
    };

    class ITcpClient : public std::enable_shared_from_this<ITcpClient> {
    public:
        //region Methods

        /**
         * Starts the client tasks such as sending & recieving data
         * @return The operation result
         */
        virtual bool Start() = 0;

        /*
         *
         * Stops the client
         * @return The operation result
         */
        virtual bool Stop() = 0;

        /**
         * Sends a response to the client
         *
         * @param code    The response code
         * @param message The response message
         * @return        The operation result
         */
        virtual void Respond(ResponseCode code, std::string_view message, std::uint8_t flags = 0) = 0;

        /**
         * Gets the client ID
         * @return The client ID
         */
        virtual const std::uint64_t Id() const noexcept = 0;

        /**
         * Sets the client ID
         * @param id The new ID
         */
        virtual void Id(const std::uint64_t& id) noexcept = 0;

        /**
         * Gets the underlaying socket of this wrapper
         * @return The socket
         */
        virtual const std::shared_ptr<boost::asio::ip::tcp::socket>& Socket() const noexcept = 0;

        //endregion

        //region Events

        /// An event handler to be invoked when the connection closes
        ezserver::shared::utils::EventHandler<
            const std::shared_ptr<ITcpClient>&,
            const boost::system::error_code&
        > ConnectionClosed;

        /// An event handler to be invoked when a new message is recieved
        ezserver::shared::utils::EventHandler<const std::shared_ptr<ITcpClient>&, std::string> MessageRecieved;

        //endregion
    };
}

#endif //EZSERVER_ITCP_CLIENT_H
