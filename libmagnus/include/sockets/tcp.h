#ifndef TCP_H
#define TCP_H

#include "server.h"

namespace LibMagnus
{
    class TCPServer : public Server
    {
    public:
        TCPServer();
        TCPServer(TCPServer&);
        TCPServer(TCPServer&&);
        ~TCPServer();
    };
}

#endif // TCP_H
