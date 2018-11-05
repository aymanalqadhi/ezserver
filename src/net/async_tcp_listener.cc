#include "net/async_tcp_listener.h"

// ==================================================== //

bool ezserver::net::AsyncTcpListener::Start()
{
    return true;
}

// ==================================================== //

bool ezserver::net::AsyncTcpListener::Stop(bool force)
{
    return false;
}

// ==================================================== //

bool ezserver::net::AsyncTcpListener::Initialize()
{
    LOG(logger_.lock(), Information)
        << "Starting listener on port " << port_
        << ", with a backlog of " << backlog_ << std::endl;

    return true;
}