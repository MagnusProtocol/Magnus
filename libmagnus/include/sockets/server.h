#ifndef SERVER_H
#define SERVER_H

#include <cinttypes>
#include "socket.h"

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
        static constexpr ulong MaxBufferLength = 4096;

        uint8_t ConnectionID;

         // Maximum length of the payload buffer
        char Buffer[Server::MaxBufferLength];

        sockaddr_in Address; // Server address

        sockaddr_in ClientAddress; // Client addres

        uint16_t Port { 3000 }; // Socket port

        unsigned long MaxConnections { 100 };

        virtual void Run();

        Server();
        Server(Server&) = default;
        Server(Server&&) = default;
        Server& operator =(Server&) = default;
        Server& operator =(Server&&) = default;
        ~Server();
    };
}

#endif // SERVER_H_
