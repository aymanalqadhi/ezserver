#ifndef EZSERVER_ASYNC_TCP_CLIENT_H
#define EZSERVER_ASYNC_TCP_CLIENT_H

#include "net/itcp_client.h"

#include <boost/asio/io_context_strand.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>

using ezserver::shared::net::ResponseCode;

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
        virtual void Respond(ResponseCode code, std::string_view message, std::uint8_t flags) override;

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
        {
            is_running_.store(false);
        }

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

        /// Client buffers
        boost::asio::streambuf buffer_;
        std::uint8_t header_buffer_[8];

        /// The client ID
        std::int64_t id_;

        /// An atomic variable to determine if the
        /// socket is running or not
        std::atomic_bool is_running_;

        /// Strart a reading process
        void StartRead();

        /// The respond method async callback
        void HandleWrite(const boost::system::error_code& err, const std::size_t& sent, const std::size_t expected);
    };
}

#endif //EZSERVER_ASYNC_TCP_CLIENT_H
