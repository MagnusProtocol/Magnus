#include "sockets/socket.hpp"

namespace LibMagnus
{
Socket::Socket()
{
    this->ID = socket(AF_INET, SOCK_STREAM, 0);
}

Socket::Socket(sockaddr_in& address)
{
    this->Bind(address);

    this->SetAddress(address);
}

Socket& Socket::SetAddress(sockaddr_in& address)
{
    this->Address.sin_family = address.sin_family;
    this->Address.sin_addr.s_addr = address.sin_addr.s_addr;
    this->Address.sin_port = address.sin_port;

    return *this;
}

Socket& Socket::Bind(sockaddr_in& address)
{
    this->SetAddress(address);

    bind(this->ID, (sockaddr*)&this->Address, sizeof(this->Address));

    return *this;
}

Socket::~Socket()
{ //
}
} // namespace LibMagnus
