#include "sockets/socket.h"

namespace LibMagnus
{
    Socket::Socket()
    {
        this->ID = socket(AF_INET, SOCK_STREAM, 0);
    }

    Socket::Socket(sockaddr_in address)
    {
        this->Bind(address);
    }

    void Socket::Bind(sockaddr_in& address)
    {
        bind(this->ID, (sockaddr*)&address, sizeof(address));
    }

    Socket::~Socket()
    {
    }
}
