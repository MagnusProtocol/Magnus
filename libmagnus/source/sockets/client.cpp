#include "sockets/client.h"

namespace LibMagnus
{
    Client& Client::SetAddress(std::string_view address)
    {
        this->DefaultServerInfo.SetAddress(address);

        return *this;
    }

    Client& Client::SetPort(uint16_t port)
    {
        this->DefaultServerInfo.SetPort(port);

        return *this;
    }

    Client& Client::Connect(std::string_view address)
    {
        this->SetAddress(address);

        if (connect(this->mSocket.ID, (sockaddr*)&this->mSocket.Address, sizeof(mSocket.Address)) < 0)
            #ifdef LOG
            std::cout << "Connection failed." << '\n';
            #endif

        this->Status = ConnectionStatus::Connected;

        return *this;
    }

    std::string_view Client::Send(std::string_view buffer)
    {
        send(this->mSocket.ID, buffer.data(), buffer.size(), 0);

        if (!this->Receive())
            #ifdef LOG
            std::cout << "Receiving failed.\n";
            #endif

        return this->ResponseBuffer;
    }

    uint8_t Client::Receive()
    {
        return recv(this->mSocket.ID, const_cast<char*>(this->ResponseBuffer.c_str()), this->BufferSize, 0);
    }

    ConnectionStatus Client::GetStatus()
    {
        return this->Status;
    }

    Client::Client() : Status(ConnectionStatus::Disconnected), DefaultServerInfo(ServerInfo())
    {
    }

    Client::Client(std::string_view address, uint16_t port)
    {
        this->SetPort(port);
        this->Connect(address);
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
