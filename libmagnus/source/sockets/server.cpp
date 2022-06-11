#include "sockets/server.h"

namespace LibMagnus
{
    Server::Server()
    {
        this->Buffer.reserve(this->MaxBufferLength);

        this->Initialize();
    }

    Server& Server::Listen()
    {
        listen(this->mSocket.ID, this->MaxConnections);
        #ifdef LOG
        std::cout << "Server socket listening on port " << this->Port << '\n';
        #endif
    }

    Server& Server::Accept()
    {
        socklen_t len = sizeof(this->ClientAddress);

        this->ConnectionID = accept(this->mSocket.ID, (sockaddr*)&this->ClientAddress, &len);
    }

    int Server::Receive()
    {
        return recv(this->ConnectionID, const_cast<char*>(this->Buffer.c_str()), this->MaxBufferLength, 0);
    }

    Server& Server::Initialize()
    {
        if (!this->Bound)
        {
            this->Address.sin_family = AF_INET;
            this->Address.sin_addr.s_addr = htonl(INADDR_ANY);
            this->Address.sin_port = htons(this->Port);

            this->mSocket = Socket(this->Address);

            this->Bound = 1;
        }

        this->Listen();
    }

    Server& Server::Send(int bytes)
    {
        send(this->ConnectionID, const_cast<char*>(this->Buffer.c_str()), bytes, 0);
    }

    int Server::Read()
    {
        int bytes = 0;

        while ((bytes = this->Receive()) > 0)
        {
            #ifdef LOG
            std::cout << "Recieved buffer: " << this->Buffer << '\n';
            #endif

            this->Send(bytes);
        }

        return bytes;
    }

    void Server::Run()
    {
        for (;;)
        {
            this->Accept();

            if (this->Read() < 0) // error check; todo: log
            {
                #ifdef LOG
                std::cout << "Closing the connection with ID ." << this->ConnectionID << '\n';
                #endif
                close(this->ConnectionID);
            }
        }
    }

    Server::~Server()
    {
    }
}