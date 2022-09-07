#ifndef TCP_H
#define TCP_H

#include "server.hpp"

namespace LibMagnus
{
class TCPServer : public Server
{
    public:
    // void Run() override;

    TCPServer();
    TCPServer(std::string_view);
    TCPServer(TCPServer&) = default;
    TCPServer(TCPServer&&) = default;
    ~TCPServer();
};
} // namespace LibMagnus

#endif // TCP_H
