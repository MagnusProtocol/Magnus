#ifndef TCP_H
#define TCP_H

#include "server.h"

namespace LibMagnus
{
    class TCPServer : public Server
    {
    public:

        void Run() override;

        TCPServer();
        TCPServer(TCPServer&);
        TCPServer(TCPServer&&);
        ~TCPServer();
    };
}

#endif // TCP_H
