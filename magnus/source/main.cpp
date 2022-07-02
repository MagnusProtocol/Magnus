#include <cmath>
#include <magnus.h>

#include "sockets/tcp.h"
#include "sockets/client.h"
#include "sys/sysinfo.h"
#include "sys/types.h"
#include <compression/lz4_wrapper.hh>
#include <future>
#include <iostream>

int main(int argc, char** argv)
{
    auto f = std::async(std::launch::async, []() {
         LibMagnus::TCPServer tcp = LibMagnus::TCPServer("127.0.0.1");

         tcp.SetBufferSize(1024000);
         tcp.Start();
        }


  );

    f.get();

    std::future<void> f1 = std::async(std::launch::async, [argv]() {
        LibMagnus::Client client = LibMagnus::Client("127.0.0.1", 3000);

        while (1)
            std::cout << client.Send(argv[1]) << '\n';
    });

    f1.get();

    return 0;
}
