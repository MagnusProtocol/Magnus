#include "sockets/client.hpp"

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

    sockaddr_in* addr = this->DefaultServerInfo.GetAddress();

    if (connect(this->mSocket.ID, (sockaddr*)addr, sizeof(*addr)) < 0)
    {
#ifdef LOG
        std::cout << "Connection failed." << '\n';
#endif

        return *this;
    }

    this->Status = ConnectionStatus::Connected;

    return *this;
}

Client& Client::Connect(ServerInfo& serverInfo)
{
    this->DefaultServerInfo = serverInfo;

    if (connect(this->mSocket.ID, (sockaddr*)this->DefaultServerInfo.GetAddress(),
                sizeof(this->DefaultServerInfo.GetAddress())) < 0)
    {
#ifdef LOG
        std::cout << "Connection failed." << '\n';
#endif

        return *this;
    }

    this->Status = ConnectionStatus::Connected;

    return *this;
}

Client& Client::Connect()
{
    if (connect(this->mSocket.ID, (sockaddr*)&this->DefaultServerInfo.Address,
                sizeof(this->DefaultServerInfo.Address)) < 0)
    {
#ifdef LOG
        perror("Connection failed");
#endif
        return *this;
    }

    this->Status = ConnectionStatus::Connected;

    return *this;
}

std::string_view Client::Send(std::string_view buffer)
{
    if (this->Status != ConnectionStatus::Connected)
        this->Connect();

    if (send(this->mSocket.ID, buffer.data(), buffer.size(), 0) < 0)
    {
        std::cout << "Buffer sending failed." << '\n';
    }

    if (!this->Receive())
        std::cout << "Receiving failed.\n";

    return this->ResponseBuffer;
}

ssize_t Client::Receive()
{
    return recv(this->mSocket.ID, const_cast<char*>(this->ResponseBuffer.c_str()), this->BufferSize,
                0);
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
    this->SetAddress(address);
    this->SetPort(port);
    this->Connect();
}

Client::Client(ServerInfo serverInfo) : DefaultServerInfo(serverInfo)
{
}

Client::Client(Client& client)
    : mSocket(client.mSocket), Status(client.Status), DefaultServerInfo(client.DefaultServerInfo)
{
}

Client::Client(Client&& client)
    : mSocket(std::move(client.mSocket)), Status(std::move(client.Status)),
      DefaultServerInfo(std::move(client.DefaultServerInfo))
{
}

Client::~Client()
{
}
} // namespace LibMagnus
