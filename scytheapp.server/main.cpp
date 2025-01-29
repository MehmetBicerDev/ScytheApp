#include "scythe_server.h"

#include "server_manager.h"
#pragma comment(lib, "scythe.util.lib")

#pragma comment(lib, "scythe.net.lib")
#pragma comment(lib, "ws2_32")
scythe::net::scythe_server* server = 0;
int main()
{
    SetConsoleTitleA("Scythe Server");

    server = new scythe::net::scythe_server(37001, 1000);


    auto debug_server = scythe::server_manager::create_server("NizternalTest", "testinvite");

    debug_server->add_voicechannel(1, "NizternalTest2");
    server->start();


    std::cin.get();
}

