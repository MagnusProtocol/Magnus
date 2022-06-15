#include "sockets/serverinfo.h"

namespace LibMagnus
{
    void ServerInfo::SetAddress(std::string_view address)
    {
        memset(&this->Address, 0, sizeof(this->Address));

        this->Address.sin_family = AF_INET;
        this->Address.sin_addr.s_addr = inet_addr(const_cast<char*>(address.data()));
    }

    void ServerInfo::SetAddress(sockaddr_in address)
    {
        memset(&this->Address, 0, sizeof(this->Address));

        this->Address.sin_family = address.sin_addr.s_addr;
        this->Address.sin_addr.s_addr = address.sin_addr.s_addr;
    }

    void ServerInfo::SetPort(uint16_t port)
    {
        this->Port = port;

        this->Address.sin_port = htons(this->Port);
    }

    ServerInfo::ServerInfo() : Port(0)
    {
        memset(&this->Address, 0, sizeof(this->Address));
    }

    ServerInfo::ServerInfo(std::string_view address, uint16_t port) : Port(port)
    {
        memset(&this->Address, 0, sizeof(this->Address));

        this->Address.sin_family = AF_INET;
        this->Address.sin_addr.s_addr = inet_addr(const_cast<char*>(address.data()));
        this->Address.sin_port = htons(this->Port);
   }

    ServerInfo::ServerInfo(const ServerInfo& serverInfo)
    {
        this->SetAddress(serverInfo.Address);
        this->SetPort(serverInfo.Port);
    }

    ServerInfo::ServerInfo(const ServerInfo&& serverInfo) : Address(std::move(serverInfo.Address)), Port(std::move(serverInfo.Port))
    {
    }

    ServerInfo::~ServerInfo()
    {
    }
}
