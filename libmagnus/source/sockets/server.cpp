#include "sockets/server.h"

namespace LibMagnus
{
    Server::Server() : Bound(false), Running(false)
    {
        this->Buffer.reserve(this->MaxBufferLength);

        this->Initialize();
    }

    Server::Server(std::string_view& address) : AddressString(address), Bound(false), Running(false)
    {
        this->Buffer.reserve(this->MaxBufferLength);

        this->SetAddress(address);
        this->Initialize();
    }

    Server& Server::Bind(sockaddr_in& address)
    {
        this->mSocket.Bind(address);

        this->Bound = true;

        return *this;
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

        return *this;
    }

    Server& Server::SetAddress(std::string_view& address)
    {
        this->Address.sin_family = AF_INET;
        this->Address.sin_addr.s_addr = inet_addr(address.data());
        this->Address.sin_port = htons(this->Port);

        this->Bind(this->Address);

        return *this;
    }

    std::string_view Server::GetAddressString()
    {
        return this->AddressString;
    }

    void Server::Stop()
    {
        #ifdef LOG
        std::cout << "Closing the connection with ID " << this->ConnectionID << ".\n";
        #endif

        close(this->mSocket.ID);

        this->Running = 0;
    }

    Server& Server::Listen()
    {
        listen(this->mSocket.ID, this->MaxConnections);

        #ifdef LOG
        std::cout << "Server socket listening on " << this->Address.sin_addr.s_addr << ":" << this->Address.sin_port << '\n';
        #endif

        return *this;
    }

    Server& Server::Accept()
    {
        socklen_t len = sizeof(this->ClientAddress);

        this->ConnectionID = accept(this->mSocket.ID, (sockaddr*)&this->ClientAddress, &len);

        return *this;
    }

    int Server::Receive()
    {
        // memset(this->Buffer.data(), 0, this->MaxBufferLength + 1);
        this->Buffer.clear();

        return recv(this->ConnectionID, this->Buffer.data(), this->MaxBufferLength, 0);
    }

    Server& Server::Send(int bytes)
    {
        send(this->ConnectionID, this->Buffer.data(), bytes, 0);

        return *this;
    }

    Server& Server::SetBufferSize(ulong size)
    {
        this->MaxBufferLength = size;

        this->Buffer.resize(size);

        return *this;
    }

    int Server::Read()
    {
        int bytes = 0;

        while ((bytes = this->Receive()) > 0)
        {
            #ifdef LOG
            std::cout << "Recieved buffer: " << this->Buffer.c_str() << '\n';
            #endif

            this->Send(bytes + 1);
        }

        return bytes;
    }

    void Server::Start()
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
            this->Read();
        }

        this->Stop();
    }

    Server::~Server()
    {
    }
}
