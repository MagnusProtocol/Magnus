#include "sockets/client.h"

namespace LibMagnus
{
    Client& Client::SetAddress(std::string address)
    {
        this->DefaultServerInfo.SetAddress(address);

        return *this;
    }

    Client& Client::SetPort(uint16_t port)
    {
        this->DefaultServerInfo.SetPort(port);

        return *this;
    }
    Client& Client::Connect(std::string)
    {
    }

    uint8_t Client::Send()
    {
    }

    uint8_t Client::Receive()
    {
    }


    Client& Client::Echo(std::string)
    {
    }

    ConnectionStatus Client::GetStatus()
    {
        return this->Status;
    }

    Client::Client() : Status(ConnectionStatus::Disconnected), DefaultServerInfo(ServerInfo())
    {
    }

    Client::Client(std::string address, uint16_t port)
    {
        this->SetAddress(address);
        this->SetPort(port);
    }

    Client::Client(ServerInfo serverInfo) : DefaultServerInfo(serverInfo)
    {
    }

    Client::Client(Client& client) : mSocket(client.mSocket),
                                    Status(client.Status),
                                    DefaultServerInfo(client.DefaultServerInfo)
    {
    }

    Client::Client(Client&& client) : mSocket(std::move(client.mSocket)),
                                      Status(std::move(client.Status)),
                                      DefaultServerInfo(std::move(client.DefaultServerInfo))
    {
    }

    Client& Client::operator =(Client& client)
    {
        Client c;

        return (c = Client(client));
    }

    Client& Client::operator =(Client&& client)
    {
        Client c;

        return (c = Client(client));
    }

    Client::~Client()
    {
    }
}
