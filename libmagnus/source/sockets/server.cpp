#include "sockets/server.h"

namespace LibMagnus
{
    Server::Server()
    {
        this->Buffer.reserve(this->MaxBufferLength);

        this->Initialize();
    }

    Server::Server(std::string_view& address)
    {
        this->Buffer.reserve(this->MaxBufferLength);

        this->SetAddress(address);
        this->Initialize();
    }

    Server& Server::SetAddress(std::string_view& address)
    {
        this->Address.sin_family = AF_INET;
        this->Address.sin_addr.s_addr = inet_addr(address.data());
        this->Address.sin_port = htons(this->Port);

        this->mSocket.Bind(this->Address);
        this->Bound = 1;
    }

    Server& Server::Listen()
    {
        listen(this->mSocket.ID, this->MaxConnections);
        #ifdef LOG
        std::cout << "Server socket listening on " << this->Address.sin_addr.s_addr << ":" << this->Address.sin_port << '\n';
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
        if (this->Running)
        {
            #ifdef LOG
            std::cout << "The server is already running." << '\n';
            #endif

            return;
        }

        this->Running = 1;

        while (this->Running)
        {
            this->Accept();

            if (this->Read() < 0) // error check; todo: log
            {
                #ifdef LOG
                std::cout << "Closing the connection with ID ." << this->ConnectionID << '\n';
                #endif
                close(this->ConnectionID);

                this->Running = 0;
            }
        }
    }

    Server::~Server()
    {
    }
}
