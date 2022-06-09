#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>

// typedef unsigned long ulong;

namespace LibMagnus
{
    class Socket
    {
    public:
        int ID;

        void Bind(sockaddr_in& address);

        Socket();

        Socket(sockaddr_in address);
        Socket(Socket&) = default;
        Socket(Socket&&) = default;

        Socket& operator =(Socket&) = default;
        Socket& operator =(Socket&&) = default;

        ~Socket();
    };
}

#endif // SOCKET_H_
