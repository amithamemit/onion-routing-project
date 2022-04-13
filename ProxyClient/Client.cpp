#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <iostream>
#include "JsonPacketSerialization.hpp"
#include "JsonPacketDeserialization.hpp"
#include "RSA_Communication.hpp"
#include <exception>

#define PROXY_PORT 8080
#define SERVER_PORT 200
#define SERVER_IP_ADDRESS "127.0.0.1"

#define CONTINUE "0"
#define GET_ROUTE "1"
#define PRINT_ROUTE "2"
#define SWITCH_LAST_RELAY "3"
#define SELECT_AMOUNT_OF_RELAYS "4"

#define MINIMUM_AMOUNT_OF_RELAYS 3
#define MAXIMUM_AMOUNT_OF_RELAYS 4

relaysPath getPathFromProxyPath(PathRequest preferences, int proxySock, RSA_Communication* rsa);
int createRoute(std::vector<nextNode> path, RSA_Communication* rsa);

int main(int argc, char const *argv[])
{
    
    RSA_Communication* rsa;
    
    int sock = 0;
    struct sockaddr_in serv_addr;
    unsigned char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PROXY_PORT);
       
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        std::cout << std::endl << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cout << std::endl << "Connection Failed \n" << std::endl;
        return -1;
    }

    //create rsa connection
    //dont forget
    
    while (true)
    {
        memset(buffer, 0, sizeof(buffer)); //reset buffer

        // create login request
        std::string username = "";
        std::string password = "";
        std::cout << "Enter username: ";
        getline(std::cin, username);
        std::cout << "Enter password: ";
        getline(std::cin, password);

        // getting request unsgined char buffer
        unsigned char* requestForProxy = nullptr; //request to send for proxy
        std::string method = "login";
        if (method == "login"){
            LoginRequest loginRequest = {};
            loginRequest.username = username;
            loginRequest.password = password;
            requestForProxy = JsonPacketSerialization::createLoginRequest(loginRequest, rsa);
        }
        else if (method == "signup"){
            SignupRequest singnupRequest = {};
            singnupRequest.username = username;
            singnupRequest.password = password;
            requestForProxy = JsonPacketSerialization::createSignupRequest(singnupRequest, rsa);
        }

        if(send(sock , requestForProxy, 1024 , 0 ) == -1){
            std::cout << "Error with server connection..." << std::endl;
            return -1;
        }

        delete requestForProxy; 

        if(read(sock , buffer, 1024) <= 0){
            std::cout << "Error with server connection..." << std::endl;
            return -1;
        }
        signupAndLoginResponse loginResponse = JsonPacketDeserialization::getLoginAndSignupResponse(buffer, rsa);
        if (loginResponse.responseID == 1)
            break;
        
    }
    relaysPath path;
    PathRequest preferences;
    preferences.relaysAmount = MINIMUM_AMOUNT_OF_RELAYS;
    while (true)
    {
        try
        {       
            memset(buffer, 0, sizeof(buffer)); //reset buffer

            unsigned char* requestForProxy = nullptr; //request to send for proxy
            std::string choice = "";
            std::cout << "Enter " << GET_ROUTE << " to get the route " <<std::endl;
            std::cout << "Enter " << PRINT_ROUTE << " to print the route " <<std::endl;
            std::cout << "Enter " << SWITCH_LAST_RELAY << " to switch last relay " <<std::endl;
            std::cout << "Enter " << SELECT_AMOUNT_OF_RELAYS << " to select amount of relays in route" <<std::endl;
            std::cout << "Enter " << CONTINUE << " to continue with selected route (if there is no route the program will create one disregarding your preferences) " <<std::endl;
            std::cout << "Insert choice here: ";
            std::cin >> choice; 
            
            if (choice == GET_ROUTE)
            {
                path = getPathFromProxyPath(preferences, sock, rsa);
                if (path.serverResponse == "-1")
                    return -1;
                Helper::printPath(path);
                std::cout << "Server response: " << path.serverResponse << std::endl;
            }

            else if (choice == PRINT_ROUTE)
            {
                Helper::printPath(path);
            }

            else if (choice == SWITCH_LAST_RELAY)
            {
                requestForProxy = JsonPacketSerialization::createAvaliableLastRelaysRequest(rsa);

                if(send(sock , requestForProxy, 1024 , 0 ) == -1)
                {
                    std::cout << "Error with server connection..." << std::endl;
                    return -1;
                }
                delete requestForProxy; 

                if(read(sock , buffer, 1024) <= 0)
                {
                    std::cout << "Error with server connection..." << std::endl;
                    return -1;
                }
                avaliableLastNodes avaliableLastRelays = JsonPacketDeserialization::avaliableLastRelaysResponse(buffer, rsa);

                int relayID;
                std::cout << "Enter chosen relay ID (enter -1 if you didn't choose any relay): ";
                std::cin >> relayID;

                if (relayID != -1)
                {    
                    if (relayID > avaliableLastRelays._countries.size() || relayID < 0)
                        std::cout << "invalid ID of node" << std::endl;
                    else
                        preferences.lastNodeCountry = avaliableLastRelays._countries[relayID];
                }            
                
            }
            else if (choice == SELECT_AMOUNT_OF_RELAYS)
            {
                std::string amountOfRelays;
                std::cout << "Select amount of relays (between 3 to 4): ";
                std::cin >> amountOfRelays;

                if (amountOfRelays.size() == 1 && amountOfRelays[0] - '0' >= MINIMUM_AMOUNT_OF_RELAYS && amountOfRelays[0] - '0' <= MAXIMUM_AMOUNT_OF_RELAYS) //maximum amount of relays is 4 and minimum amount is 3
                {
                    preferences.relaysAmount = amountOfRelays[0] - '0';
                }
                else
                {
                    std::cout << "Error: Invalid amount of relays" << std::endl;
                }
            }
            else if (choice == CONTINUE)
            {
                for (int i = 0; i < 3 || path._path.size() > 0; i++) //the program will try to create a route 3 times and if it doesnt work it will quit
                {
                    path = getPathFromProxyPath(preferences, sock, rsa);               
                }
                std::cout << path.serverResponse << std::endl;
                if (path._path.size() == 0)
                {
                    return -1;
                }
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return -1;
        }
        
    }
    std::vector<RSA_Communication*> rsaHandlers;
    for(int i = 0; i < path._path.size(); i++){
        rsaHandlers.push_back(new RSA_Communication("", i)); //need to use it later
    }

    int firstRelaySocket = createRoute(path._path, rsa);
    
    nextNode nextDestination;

    std::cout << "Connecting to the last node..." << std::endl;
    nextDestination.encryptKey = "";
    nextDestination.ipAdress = SERVER_IP_ADDRESS;
    nextDestination.port = SERVER_PORT;
    unsigned char* buffer = JsonPacketSerialization::createNextNodeResult(nextDestination, rsa);
    if(send(firstRelaySocket, buffer, 1024, 0) == -1)
    { 
        close(firstRelaySocket);
        delete buffer;
        throw SocketCreationException();
    }
    delete buffer;

    if(send(firstRelaySocket, "hi", 1024, 0) == -1)
    { 
        close(firstRelaySocket);
        return false;
    }

    memset(buffer, 0, 1024);

    if(read(firstRelaySocket , buffer, 1024) <= 0){
        close(firstRelaySocket);
        std::cout << "Error with read message from client. Client disconnected..." << std::endl;
    }

    unsigned char* decrypted = buffer;
    unsigned char* toClean;
    for(int i = 0; i < 3; i++){
        toClean = decrypted;
        decrypted = rsaHandlers[i]->decrypt(decrypted); //need to fix in line 207
        delete toClean;
    }

    std::cout << "got " << decrypted << " from server." << std::endl;

    delete decrypted;

    std::cout << "Closing socket..." << std::endl;
    close(sock);
    return 0;
}

/*
this function asks the proxy for the route
input: route prefernces
output: route (proxy response)
*/
relaysPath getPathFromProxyPath(PathRequest preferences, int proxySock, RSA_Communication* rsa)
{
    relaysPath path;
    unsigned char* buffer = JsonPacketSerialization::createPathRequest(preferences, rsa);
    if(send(proxySock, buffer, 1024 , 0 ) == -1)
    {
        throw SocketDisconnectedException();
    }

    memset(buffer, 0, sizeof(buffer)); //reset buffer    

    if(read(proxySock , buffer, 1024) <= 0){
        throw SocketDisconnectedException();
    }

    path = JsonPacketDeserialization::getRouteResponse(buffer, rsa);
    delete(buffer);

    return path;
} 

/*
this function connects to all relays in the path sent to it
input: path
output: first node socket
*/ 
int createRoute(std::vector<nextNode> path, RSA_Communication* rsa)
{
    int newSocket = 0;
    
    struct sockaddr_in serv_addr;
    if ((newSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        throw SocketCreationException();      
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(path[0].port);
    
    // creating a connection to the first node
    if(inet_pton(AF_INET, path[0].ipAdress.c_str(), &serv_addr.sin_addr) <=0 )
    {
        throw SocketCreationException();
    }
    std::cout << "Connecting to the 1 node..." << std::endl;
    if (connect(newSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        throw SocketCreationException();
    }
    for (int i = 1; i < path.size(); i++)
    {
        nextNode nextDestination;

        std::cout << "Connecting to the " << i + 1 << " node..." << std::endl;
        nextDestination.encryptKey = path[i].encryptKey;
        nextDestination.ipAdress = path[i].ipAdress;
        nextDestination.port = path[i].port;
        unsigned char* buffer = JsonPacketSerialization::createNextNodeResult(nextDestination, rsa);
        if(send(newSocket, buffer, 1024, 0) == -1)
        { 
            close(newSocket);
            delete buffer;
            throw SocketCreationException();
        }
        delete buffer;
    }
    return newSocket;
}

