#ifndef EZSERVER_ASYNC_TCP_CLIENT_H
#define EZSERVER_ASYNC_TCP_CLIENT_H

#include "net/itcp_client.h"
#include <boost/asio/io_context_strand.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace ezserver::net
{
    class AsyncTcpClient : public ezserver::shared::net::ITcpClient
    {
    public:

        //region Base class methods

        /// Base class method override
        virtual bool Start() override;

        /// Base class method override
        virtual bool Stop() override;

        /// Base class method override
        virtual inline const std::uint64_t Id() const noexcept override { return id_; }

        /// Base class method override
        virtual inline void Id(const uint64_t& id) noexcept override { id_ = id; }

        //endregion

        /// Virtual Destructor
        virtual ~AsyncTcpClient()
        {}

        /**
         * Default constructor
         * @param client_socket The socket to use
         */
        AsyncTcpClient(std::shared_ptr<boost::asio::ip::tcp::socket> client_socket)
            : client_socket_(client_socket), strand_(client_socket->get_io_context())
        {}

        /**
         * Alternative constructor to construct a new socket
         * using a given io_context
         * @param io The io_context to use
         */
        AsyncTcpClient(boost::asio::io_context &io)
            : AsyncTcpClient(std::make_shared<boost::asio::ip::tcp::socket>(io))
        {}

        /**
         * The the low level socket
         * @return a pointer to the inner socket
         */
        virtual const std::shared_ptr<boost::asio::ip::tcp::socket> &Socket() const noexcept override
        {
            return client_socket_;
        }

    private:
        /// The inner socket used by this wrapper
        std::shared_ptr<boost::asio::ip::tcp::socket> client_socket_;

        /// The synchronization object for this connection
        boost::asio::io_context::strand strand_;

        /// The client ID
        std::int64_t id_;

        void StartRead();

        void StartWrite();
    };
}

#endif //EZSERVER_ASYNC_TCP_CLIENT_H
