#ifndef SERVER_H
#define SERVER_H

#include "socket.hpp"
#include <cinttypes>
#include <deque>
#include <string>
#include <string_view>

#define LOG

#ifdef LOG
#include <iostream>
#endif

typedef unsigned long ulong;

namespace LibMagnus {
/*
 * Abstract class containing the structor of a server.
 */
class Server {
protected:
    Socket mSocket; // Default Socket instance

    bool Bound; // Binding status

    virtual Server& Bind(sockaddr_in&);
    virtual Server& Listen();
    virtual Server& Accept();
    virtual Server& Send(int);
    virtual Server& Initialize();
    virtual int Receive();
    virtual int Read(); // Reads the buffer sent by client.

public:
    ulong MaxBufferLength { 2048 };

    uint8_t ConnectionID;

    bool Running;

    // Maximum length of the payload buffer
    std::string Buffer;

    std::deque<std::string_view> BufferStack;

    Server& SetAddress(std::string_view&);

    Server& SetBufferSize(ulong);

    std::string_view GetAddressString();

    sockaddr_in Address; // Server address

    sockaddr_in ClientAddress; // Client address

    std::string_view AddressString {};

    uint16_t Port { 3000 }; // Socket port

    unsigned long MaxConnections { 100 };

    virtual void Start();
    virtual void Stop();

    Server();
    Server(std::string_view&);
    Server(Server&) = default;
    Server(Server&&) = default;
    ~Server();
};
} // namespace LibMagnus

#endif // SERVER_H
