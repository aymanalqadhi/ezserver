#ifndef EZSERVER_CLIENT_ACCEPTED_EVENTARGS_H
#define EZSERVER_CLIENT_ACCEPTED_EVENTARGS_H

#include "utils/event_args.h"

#include <net/itcp_client.h>
#include <boost/system/error_code.hpp>

namespace ezserver::shared::net
{
    class ClientAcceptedEventArgs : public ezserver::shared::utils::EventArgs
    {
    public:

        /**
         * Default constructor
         */
        ClientAcceptedEventArgs(
            std::shared_ptr<ezserver::shared::net::ITcpClient>& client,
            const boost::system::error_code& error_code
        ) : client_(client), error_code_(error_code) {}


        /// Gets a pointer to the client socket
        std::shared_ptr<ezserver::shared::net::ITcpClient> Client() const noexcept
        { return client_; }

        /// Gets the error code
        const boost::system::error_code& ErrorCode() const noexcept
        { return error_code_; }

    private:
        /**
         * A pointer to the client that was accepted
         */
        std::shared_ptr<ezserver::shared::net::ITcpClient>& client_;

        /**
         * The operation error code (if there was any)
         */
        const boost::system::error_code& error_code_;
    };
}

#endif //EZSERVER_CLIENT_ACCEPTED_EVENTARGS_H
