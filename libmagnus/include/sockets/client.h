#ifndef CLIENT_H
#define CLIENT_H

#include "socket.h"
#include "serverinfo.h"
#include <string_view>

namespace LibMagnus
{
    enum class ConnectionStatus
    {
        Disconnected,
        Connected
    };

    /*
    * Abstract class containing the structor of a server.
    */
    class Client
    {
    protected:
        Socket mSocket;

        ConnectionStatus Status;

        virtual uint8_t Send();
        virtual uint8_t Receive();
    public:
        ServerInfo DefaultServerInfo;

        ConnectionStatus GetStatus(); // Returns the connection status of the Client.

        Client& SetAddress(std::string_view);
        Client& SetPort(uint16_t);

        virtual Client& Connect(std::string_view);
        virtual Client& Echo(std::string_view); // Sends the provided string_view to the connected server

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
