#include <iostream>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <map>
#include <thread>
#include <unistd.h>
#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <chrono>



int main()
{
    int server_fd, newSocket, serverSocket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
       
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( 200 );
       
    // Forcefully attaching socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    serverSocket = server_fd;
    std::cout << "Listening on port " << 200 << "..." << std::endl;

    // loop that check for new clients.
    while (true)
    {
        if ((newSocket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
        {
            //perror("accept");
            //exit(EXIT_FAILURE);
            std::cout << "client connection invalid" <<std::endl;
            close(newSocket);
            break;
        }
        
        std::cout << "New client accepted..." << std::endl;
        
        unsigned char* buffer = new unsigned char;
        if(read(newSocket , buffer, 1024) <= 0){
            close(newSocket);
            std::cout << "Error with read message from client. Client disconnected..." << std::endl;
        }
        std::cout << "got " << buffer << " from client" << std::endl;
        delete buffer;

        std::cout << "sending hello to client..." << std::endl;

        unsigned char* response = (unsigned char*)"hello";

        if(send(newSocket, response, 1024, 0) == -1)
        { //changed from decrypted ro toDEcryptBuffer
            close(newSocket);
            return false;
        }
        
    }
}