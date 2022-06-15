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
    class ServerInfo
    {
    private:
        sockaddr_in Address;

        uint16_t Port;

    public:
        void SetAddress(std::string_view);
        void SetAddress(sockaddr_in);
        void SetPort(uint16_t);

        ServerInfo();
        ServerInfo(std::string_view, uint16_t);
        ServerInfo(const ServerInfo&);
        ServerInfo(const ServerInfo&&);
        ServerInfo& operator =(ServerInfo&);
        ServerInfo& operator =(ServerInfo&&);
        ~ServerInfo();
    };
}

#endif //
