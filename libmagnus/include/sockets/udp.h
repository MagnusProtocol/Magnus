#ifndef UDP_H
#define UDP_H

#include "server.h"

namespace Magnus
{
    class UDPServer
    {
    public:
        UDPServer() = default;

        UDPServer(UDPServer&) = default;
        UDPServer(UDPServer&&) = default;

        UDPServer& operator =(UDPServer&) = default;
        UDPServer& operator =(UDPServer&&) = default;

        ~UDPServer() = default;
    };
}

#endif // UDP_H_
