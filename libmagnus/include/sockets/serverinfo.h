#ifndef SERVERINFO_H
#define SERVERINFO_H

#include "socket.h"
#include <cstring>
#include <cinttypes>
#include <string_view>
#include <utility>

namespace LibMagnus
{
    /*
     * Stores the related server info.
     */
    struct ServerInfo
    {
    public:
        sockaddr_in Address;

        uint16_t Port;

        void SetAddress(std::string_view);
        void SetAddress(sockaddr_in);
        void SetPort(uint16_t);

        sockaddr_in* GetAddress();
        uint16_t GetPort();

        ServerInfo();
        ServerInfo(std::string_view, uint16_t);
        ServerInfo(const ServerInfo&);
        ServerInfo(ServerInfo&&);

        ServerInfo& operator =(const ServerInfo&) noexcept;
        ServerInfo& operator =(ServerInfo&&) noexcept;

        ~ServerInfo();
    };
}

#endif //
