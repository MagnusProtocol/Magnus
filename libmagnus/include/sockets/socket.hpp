#ifndef SOCKET_H
#define SOCKET_H

#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// typedef unsigned long ulong;

namespace LibMagnus
{
class Socket
{
    public:
    int ID;

    sockaddr_in Address;

    Socket& SetAddress(sockaddr_in&);
    Socket& Bind(sockaddr_in&);

    Socket();

    Socket(sockaddr_in& address);
    Socket(Socket&) = default;
    Socket(Socket&&) = default;

    Socket& operator=(Socket&) = default;
    Socket& operator=(Socket&&) = default;

    ~Socket();
};
} // namespace LibMagnus

#endif // SOCKET_H_
