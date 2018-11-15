#include "net/async_tcp_client.h"

#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/system/error_code.hpp>
#include <boost/asio/io_context_strand.hpp>

#include <sstream>

// ============================================================== //

bool ezserver::net::AsyncTcpClient::Start()
{
    StartRead();
    return true;
}

// ============================================================== //

bool ezserver::net::AsyncTcpClient::Stop()
{
    boost::system::error_code err;

    // Cleanly close the connection
    if (client_socket_->is_open())
    {
        client_socket_->shutdown(boost::asio::socket_base::shutdown_both, err);
        client_socket_->close(err);
    }

    return err == boost::system::errc::success;
}

// ============================================================== //

void ezserver::net::AsyncTcpClient::StartRead()
{
    // Create a shared pointer of this in order
    // to keep the object alive
    auto client = shared_from_this();

    // Start an asynchronous read job
    boost::asio::async_read_until(
        *client_socket_, buffer_, '\n',
        strand_.wrap([this, client] (const boost::system::error_code& err, const std::size_t& rec) {

            // If an error was found, fire the connection loss
            // event handler, and close the connection
            if (err)
            {
                // Raise the connection loss event, and then return
                ConnectionClosed.Invoke(client, err);
                return;
            }

            // Raise the receiving event
            MessageRecieved.Invoke(
                client,
                std::move(std::string(std::istreambuf_iterator<char>(&buffer_), std::istreambuf_iterator<char>()))
            );

            // Restart the reading loop
            StartRead();
        })
    );
}

// ============================================================== //
