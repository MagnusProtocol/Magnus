#ifndef SERVER_H
#define SERVER_H

#include <cinttypes>
#include <string>
#include <string_view>
#include "socket.h"

#define LOG

#ifdef LOG
#include <iostream>
#endif

namespace LibMagnus
{
    /*
    * Abstract class containing the structor of a server.
    */
    class Server
    {
    protected:
        Socket mSocket; // Default Socket instance

        uint8_t Bound; // Binding status

        virtual Server& Listen();
        virtual Server& Accept();
        virtual Server& Send(int);
        virtual Server& Initialize();
        virtual int Receive();
        virtual int Read(); // Reads the buffer sent by client.

    public:
        ulong MaxBufferLength { 4096 };

        uint8_t ConnectionID;

        uint8_t Running;

        // Maximum length of the payload buffer
        std::string Buffer;

        Server& SetAddress(std::string_view&);

        sockaddr_in Address; // Server address

        sockaddr_in ClientAddress; // Client address

        uint16_t Port { 3000 }; // Socket port

        unsigned long MaxConnections { 100 };

        virtual void Run();

        Server();
        Server(std::string_view&);
        Server(Server&) = default;
        Server(Server&&) = default;
        Server& operator =(Server&) = default;
        Server& operator =(Server&&) = default;
        ~Server();
    };
}


#endif // SERVER_H
