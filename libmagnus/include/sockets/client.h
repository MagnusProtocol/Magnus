#ifndef CLIENT_H
#define CLIENT_H

#include "socket.h"
#include "serverinfo.h"
#include <string>
#include <string_view>

#define LOG

#ifdef LOG
#include <iostream>
#endif

namespace LibMagnus
{
    enum class ConnectionStatus
    {
        Connected,
        Disconnected
    };

    /*
    * Abstract class containing the structor of a server.
    */
    class Client
    {
    protected:
        Socket mSocket;

        ConnectionStatus Status;

        std::string ResponseBuffer;

        uint64_t BufferSize { 4096 };

        virtual uint8_t Receive();

    public:
        ServerInfo DefaultServerInfo;

        ConnectionStatus GetStatus(); // Returns the connection status of the Client.

        Client& SetBufferSize(uint64_t);
        Client& SetAddress(std::string_view);
        Client& SetPort(uint16_t);

        virtual Client& Connect();
        virtual Client& Connect(std::string_view);
        virtual Client& Connect(ServerInfo&);
        virtual std::string_view Send(std::string_view); // Sends the provided string_view to the connected server

        Client();
        Client(std::string_view, uint16_t); // with default server address
        Client(ServerInfo);
        Client(Client&);
        Client(Client&&);
        Client& operator =(Client&);
        Client& operator =(Client&&);
        ~Client();
    };
}


#endif // CLIENT_H_
