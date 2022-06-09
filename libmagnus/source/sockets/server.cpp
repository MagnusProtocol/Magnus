#include "sockets/server.h"

namespace LibMagnus
{
    Server::Server()
    {
        this->Initialize();
    }

    Server& Server::Listen()
    {
        listen(this->mSocket.ID, this->MaxConnections);
    }

    Server& Server::Accept()
    {
        socklen_t len = sizeof(this->ClientAddress);

        this->ConnectionID = accept(this->mSocket.ID, (sockaddr*)&this->ClientAddress, &len);
    }

    int Server::Receive()
    {
        recv(this->ConnectionID, this->Buffer, Server::MaxBufferLength, 0);
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

    void Server::Run()
    {
    }


    Server::~Server()
    {
    }
}
