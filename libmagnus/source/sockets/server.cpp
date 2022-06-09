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
        return recv(this->ConnectionID, this->Buffer, this->MaxBufferLength, 0);
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
        send(this->ConnectionID, this->Buffer, bytes, 0);
    }

    int Server::Read()
    {
        int bytes = 0;

        while ((bytes = this->Receive()) > 0)
        {
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
                close(this->ConnectionID);
        }
    }

    Server::~Server()
    {
    }
}
