#include "sockets/serverinfo.hpp"

namespace LibMagnus {
void ServerInfo::SetAddress(std::string_view address)
{
    memset(&this->Address, 0, sizeof(this->Address));

    this->Address.sin_family = AF_INET;
    this->Address.sin_addr.s_addr = inet_addr(const_cast<char*>(address.data()));
}

void ServerInfo::SetAddress(sockaddr_in address)
{
    memset(&this->Address, 0, sizeof(this->Address));

    this->Address.sin_family = address.sin_family;
    this->Address.sin_addr.s_addr = address.sin_addr.s_addr;
}

sockaddr_in* ServerInfo::GetAddress()
{
    return &this->Address;
}

uint16_t ServerInfo::GetPort()
{
    return this->Port;
}

void ServerInfo::SetPort(uint16_t port)
{
    this->Port = port;

    this->Address.sin_port = htons(this->Port);
}

ServerInfo::ServerInfo()
    : Port(0)
{
    memset(&this->Address, 0, sizeof(this->Address));
}

ServerInfo::ServerInfo(std::string_view address, uint16_t port)
    : Port(port)
{
    this->SetAddress(address);
    this->SetPort(port);
}

ServerInfo::ServerInfo(const ServerInfo& serverInfo)
{
    this->SetAddress(serverInfo.Address);
    this->SetPort(serverInfo.Port);
}

ServerInfo::ServerInfo(ServerInfo&& serverInfo)
{
    this->Address.sin_family = std::move(serverInfo.Address.sin_family);
    this->Address.sin_addr.s_addr = std::move(serverInfo.Address.sin_addr.s_addr);
    this->Address.sin_port = std::move(serverInfo.Address.sin_port);

    this->Port = std::move(serverInfo.Port);
}

ServerInfo& ServerInfo::operator=(const ServerInfo& serverInfo) noexcept
{
    ServerInfo _serverInfo;

    return (_serverInfo = ServerInfo(serverInfo));
}

ServerInfo& ServerInfo::operator=(ServerInfo&& serverInfo) noexcept
{
    ServerInfo _serverInfo;

    return (_serverInfo = ServerInfo(serverInfo));
}

ServerInfo::~ServerInfo()
{
}
}
