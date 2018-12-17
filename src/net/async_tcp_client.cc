#include "net/async_tcp_client.h"

#include <boost/system/error_code.hpp>

#include <boost/asio/write.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context_strand.hpp>
#include <boost/asio/placeholders.hpp>

#include <cstdio>

using namespace std::placeholders;

// ============================================================== //

bool ezserver::net::AsyncTcpClient::Start()
{
    // Return false if the socket is already started
    if (is_running_) return false;

    // set the client status as running
    is_running_.store(true);

    // Start a reading task
    StartRead();

    return true;
}

// ============================================================== //

bool ezserver::net::AsyncTcpClient::Stop()
{
    // Return false if already stopped
    if (!is_running_) return false;

    // Start a reading task, and set the client status as stopped
    is_running_.store(false);

    // Temporary error code variable
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

void ezserver::net::AsyncTcpClient::Respond(ResponseCode code, std::string_view message, std::uint8_t flags)
{
    std::uint8_t header[8]
    {
        // Response code and response flags
        static_cast<std::uint8_t>(code), flags,

        // Preserved 2 bytes
        0x0, 0x0,

        // Message length distributed on 4 bytes
        static_cast<std::uint8_t>((message.length() & 0x000000FF)),
        static_cast<std::uint8_t>((message.length() & 0x0000FF00) >> 0x4),
        static_cast<std::uint8_t>((message.length() & 0x00FF0000) >> 0x8),
        static_cast<std::uint8_t>((message.length() & 0xFF000000) >> 0xC)
    };

    // Send the header
    client_socket_->async_send(boost::asio::buffer(header, 8),
        strand_.wrap(std::bind(&ezserver::net::AsyncTcpClient::HandleWrite, this, _1, _2, 8))
    );

    // Send the message itself
    client_socket_->async_send(boost::asio::buffer(message),
       strand_.wrap(std::bind(&ezserver::net::AsyncTcpClient::HandleWrite, this, _1, _2, message.length()))
    );
}

// ============================================================== //

void ezserver::net::AsyncTcpClient::StartRead()
{
    // TODO:
    // Change this to use the server approach

    // Return if the connection was closed
    if (!is_running_ || !client_socket_->is_open()) return;

    // Create a shared pointer of this in order
    // to keep the object alive
    auto client = shared_from_this();

    client_socket_->async_receive(boost::asio::buffer(header_buffer_, 8),
        [this, client] (const boost::system::error_code& err, const std::size_t& rec) {

            // Disconnect if there were any errors
            if (err)
            {
                // Raise the connection loss event, and then return
                client->ConnectionClosed.Invoke(client, err);
                return;
            }

            // Disconnect if the header size does not equal to 8
            if (rec != 8)
            {
                client->ConnectionClosed.Invoke(client, boost::asio::error::message_size);
                return;
            }

            // Get the message length
            std::uint32_t msg_length =
                static_cast<std::uint8_t>(header_buffer_[4])        |
                static_cast<std::uint8_t>(header_buffer_[5]) << 0x4 |
                static_cast<std::uint8_t>(header_buffer_[6]) << 0x8 |
                static_cast<std::uint8_t>(header_buffer_[7]) << 0xC;

            // A dynamically allocated buffer
            auto buffer = new char[msg_length];

            // Start an asynchronous receving task to recevie the message
            // with the size of msg_length
            client_socket_->async_receive(boost::asio::buffer(buffer, msg_length),
                strand_.wrap([this, client, msg_length, buffer] (const boost::system::error_code& err, const std::size_t& rec) {

                    // If an error was found, fire the connection loss
                    // event handler, and close the connection
                    if (err)
                    {
                        // Delete the buffer
                        if (buffer) delete[] buffer;

                        // Raise the connection loss event, and then return
                        client->ConnectionClosed.Invoke(client, err);
                        return;
                    }

                    // Disconnect if the received message size does not equal to the expected size
                    if (msg_length != rec)
                    {
                        // Delete the buffer
                        if (buffer) delete[] buffer;

                        // Raise the connection loss event, and then return
                        client->ConnectionClosed.Invoke(client, boost::asio::error::message_size);
                        return;
                    }

                    // Raise the receiving event
                    client->MessageRecieved.Invoke(
                        client,
                        std::move(std::string(buffer))
                    );

                    // Restart the reading loop
                    StartRead();
                })
            );
        }
    );

}

// ============================================================== //

void ezserver::net::AsyncTcpClient::HandleWrite(
    const boost::system::error_code &err,
    const std::size_t &sent,
    const std::size_t expected)
{
    // Disconnect the client if the message could not
    // be sent
    if (err || sent != expected)
       Stop();
}
