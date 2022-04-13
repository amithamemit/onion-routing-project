#include <iostream>
#include "Server.hpp"
#include "ThreadPool.hpp"
#include "SSHConnection.hpp"

int main(){
    Server* server = new Server();

    server->run();

    std::string userInput = "";
    while (true)
    {
        getline(std::cin, userInput);
        if(userInput == "quit"){
            server->shutdown();
            break;
        }
    }

    return 0;
}