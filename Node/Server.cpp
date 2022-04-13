#include "Server.hpp"


/*
server constructor
*/
Server::Server(int port){
    this->m_port = port;
    this->m_serverSocket = 0;
    this->m_databaseServerSocket = this->connectToDatabaseServer();
    this->m_proxySocket = this->connectToProxyServer();
    this->m_serverDecryptRSA = new RSA_Communication(this->m_port);
    // check if the connection to the database server not succeed
    if(m_databaseServerSocket == -1){
        std::string errorMessage = Helper::getCurrentTime() + " Connection failed to the databse server";
        throw std::runtime_error(errorMessage);
    }
    // check if the connection to the proxy server not succeed
    if(m_proxySocket == -1){
        std::string errorMessage = Helper::getCurrentTime() + " Connection failed to the proxy server";
        throw std::runtime_error(errorMessage);
    }
    this->m_threadPool = new ThreadPool(THREADS_COUNT, this);
}

/*
server destructor
*/
Server::~Server(){
    delete this->m_threadPool;
    delete this->m_serverDecryptRSA;
}

/*
method that start the server.
Input: none.
Output: none.
*/
void Server::run(){
    std::thread bindAndListenThread = std::thread(&Server::bindAndListen, this);
    bindAndListenThread.detach();

    //std::thread communicationWithProxyThread = std::thread(&Server::communicationWithProxy, this);
    //communicationWithProxyThread.detach();
}

/*
method that close the server.
also close all client sockets.
Input: none.
Output: none.
*/
void Server::shutdown(){
    std::cout << "Closing server socket..." << std::endl;
    close(this->m_serverSocket);
    std::cout << "Closing proxy server socket..." << std::endl;
    close(this->m_proxySocket);
    std::cout << "Closing database server socket..." << std::endl;
    close(this->m_databaseServerSocket);

    
    while(!this->m_conversations.empty())
    {
        ::shutdown(this->m_conversations.front().socketToDecrypt, SHUT_RDWR);
        ::shutdown(this->m_conversations.front().socketToEncrypt, SHUT_RDWR);
        std::cout << "Closing client sockets number %d, %d \n", this->m_conversations.front().socketToDecrypt, this->m_conversations.front().socketToEncrypt;
        this->m_conversations.pop();
    }
    //to show the socket closing
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

/*
method that start bind and listen.
when new client accepted, the method create LoginRequestHandler to check access to this server.
the method create to the new client new thread that listen to his requests.
the thread response to the user requests.
Input: none.
Output: none.
*/
void Server::bindAndListen(){
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    std::thread createNewConvesrationThread = std::thread(&Server::createPathThread, this);
    createNewConvesrationThread.detach(); //thread to open conversation only when a message is waiting
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
    address.sin_port = htons( this->m_port );
       
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

    this->m_serverSocket = server_fd;
    std::cout << Helper::getCurrentTime() << "Listening on port " << this->m_port << "..." << std::endl;

    // loop that check for new clients.
    while (true)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        else{
            std::cout << Helper::getCurrentTime() << "New client accepted..." << std::endl;
            this->m_NewSocketLock.lock();
            this->m_socketsToStartConvesrations.push(new_socket);
            this->m_NewSocketLock.unlock();
        }
    }
}

/*
Method that connect to the database server.
Input: None.
Output: database connection socket.
*/
int Server::connectToDatabaseServer(){
    int sock = 0;
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << Helper::getCurrentTime() << "Socket creation error for database server." << std::endl;
        return -1;
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(DATABASE_SERVER_PORT);
       
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, DATABASE_IP_ADDRESS, &serv_addr.sin_addr) <=0 )
    {
        return -1;
    }
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        return -1;
    }
    std::cout << Helper::getCurrentTime() << "Connection to database server succeed" << std::endl;
    return sock;
}

/*
Method that connects to the proxy server.
Input: None.
Output: proxy connection socket.
*/
int Server::connectToProxyServer(){
    int sock = 0;
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << Helper::getCurrentTime() << "Socket creation error for proxy server." << std::endl;
        return -1;
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PROXY_SERVER_PORT);
       
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, PROXY_SERVER_ADDRESS, &serv_addr.sin_addr) <=0 )
    {
        return -1;
    }
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        return -1;
    }
    std::cout << Helper::getCurrentTime() << "Connection to proxy server succeed" << std::endl;
    return sock;
}


/*
this function handles the message that is waiting in the socket
input: communication data (sockets and rsa handler) and current server pointer (access own variables in static function)
output: if the function succeeded or not
*/
bool Server::handleRequest(CommunicationData communicationData, Server* server){
    unsigned char toDecryptBuffer[BUFFER_SIZE] = {0};
    unsigned char toEncryptBuffer[BUFFER_SIZE] = {0};
    bool decryptMessage = false;
    bool encryptMessage = false;

    // check if first socket sent a message
    int bytes_available = 0;
    ioctl(communicationData.socketToDecrypt, FIONREAD, &bytes_available);
    if(bytes_available != 0){
        decryptMessage = true;
        // if socket disconnected
        if(read(communicationData.socketToDecrypt , toDecryptBuffer, BUFFER_SIZE) <= 0){
            close(communicationData.socketToEncrypt);
            close(communicationData.socketToDecrypt);
            return false;
        }
    }

    // check if the second socket sent a message
    ioctl(communicationData.socketToEncrypt, FIONREAD, &bytes_available);
    if(bytes_available != 0){
        encryptMessage = true;
        // if socket disconnected
        if(read(communicationData.socketToEncrypt, toEncryptBuffer, BUFFER_SIZE) <= 0){
            close(communicationData.socketToEncrypt);
            close(communicationData.socketToDecrypt);
            return false;
        }
    }


    if (encryptMessage){
        // encrypt the data using the public key of the user
        std::cout << "encrypting data..." << std::endl;
        unsigned char* encrypted = communicationData.RsaHelper->encrypt(toEncryptBuffer);
        // send response to client
        if(send(communicationData.socketToDecrypt , encrypted, BUFFER_SIZE, 0) == -1){ //i changed it from encrypted to toEncrypt buffer for testing
            close(communicationData.socketToEncrypt);
            close(communicationData.socketToDecrypt);
            return false;
        }
        
    }
    if(decryptMessage){
        // decrypt the data using the private key of the current node
        std::cout << "decrypting data..." << std::endl;
        //unsigned char* decrypted = server->m_serverDecryptRSA->decrypt(toDecryptBuffer);
        // send the decrypted message
        if(send(communicationData.socketToEncrypt, toDecryptBuffer, BUFFER_SIZE, 0) == -1){ //changed from decrypted ro toDEcryptBuffer
            close(communicationData.socketToEncrypt);
            close(communicationData.socketToDecrypt);
            return false;
        }
    }
    return true;
}

/*
* this functions runs as a thread and creates a path for each socket that has a message waiting fo next destination and places the sockets with a path in a different queue
* input: none
* output: none
*/
void Server::createPathThread()
{
    int newSocket; //socket to check temp variable
    while(true)
    {
        this->m_NewSocketLock.lock();
        if(this->m_socketsToStartConvesrations.empty()){
            this->m_NewSocketLock.unlock();
            continue;
        }
        newSocket = this->m_socketsToStartConvesrations.front();
        this->m_socketsToStartConvesrations.pop();
        this->m_NewSocketLock.unlock();

        int bytes_available = 0;
        ioctl(newSocket, FIONREAD, &bytes_available);

        if(bytes_available == 0){
            //need to make sure later that socket is not opened without any message for too many times.
            this->m_NewSocketLock.lock();
            this->m_socketsToStartConvesrations.push(newSocket);
            this->m_NewSocketLock.unlock();
        }

        else{
            CommunicationData newConversation = this->buildPath(newSocket);
            if (newConversation.socketToDecrypt != -1 && newConversation.socketToEncrypt != -1)
            {
                this->m_conversationsLock.lock();
                this->m_conversations.push(newConversation);
                this->m_conversationsLock.unlock();
            }
            else{
                close(newSocket);
            }
        }



    }
}

/*
a function that gets the first message from client and builds the path
input: socket of the client
output: the path data
*/
CommunicationData Server::buildPath(int clientSocket){
    CommunicationData communicationData;
    std::string currentConversationNodePublicKey;
    std::string currentClientPublicKey;

    unsigned char buffer[BUFFER_SIZE];

    // getting the buffer
    if(read(clientSocket , buffer, BUFFER_SIZE) <= 0){
        // return conversation data with -1 as sockets to indicate error
        communicationData.socketToDecrypt = -1;
        communicationData.socketToEncrypt = -1;
        return communicationData;
    }

    // getting from the buffer the port and ip address 
    nextNode nextRelay = JsonPacketDeserialization::getNextNode(buffer);

    std::cout << Helper::getCurrentTime() << "Bulding path: Next Ip address, port: " << nextRelay.ipAdress << ", " << nextRelay.port << std::endl;


    // connect to the next server / relay
    int newSocket = 0;
    struct sockaddr_in serv_addr;
    if ((newSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << Helper::getCurrentTime() << "Socket creation error for next relay." << std::endl;
        communicationData.socketToDecrypt = -1;
        communicationData.socketToEncrypt = -1;
        return communicationData;
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(nextRelay.port);
       
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, nextRelay.ipAdress.c_str(), &serv_addr.sin_addr) <=0 )
    {
        communicationData.socketToDecrypt = -1;
        communicationData.socketToEncrypt = -1;
        return communicationData;
    }
   
    if (connect(newSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        communicationData.socketToDecrypt = -1;
        communicationData.socketToEncrypt = -1;
        return communicationData;
    }

    // create communication data struct
    communicationData.socketToEncrypt = newSocket;
    communicationData.socketToDecrypt = clientSocket;
    communicationData.RsaHelper = new RSA_Communication(nextRelay.encryptKey, this->m_port);
    //----communicationData.encryptKey = nextRelay.encryptKey;


    // need to send the node public key
    std::string nodePublicKey = communicationData.RsaHelper->getMyPublicKey();

    return communicationData;
    

}

/*
* this function updates the proxy about amount of connected users - runs until relay is closed as a thread
* input: none
* output: none
*/
void Server::communicationWithProxy()
{
    auto start = std::chrono::high_resolution_clock::now();
    int lastAmountOfUsersSent = 0;
    int currentAmountOfUsers;
    while (true)
    {
        //normal communication with proxy


        auto currTime = std::chrono::high_resolution_clock::now();
        //each 45 seconds or more send an update message containing amount of users connected to node to proxy (send only if there was a major change in the amount of users)
        if (std::chrono::duration_cast<std::chrono::seconds>(currTime - start).count() >= 5)
        {
            
            this->m_conversationsLock.lock();
            currentAmountOfUsers = this->m_conversations.size();
            this->m_conversationsLock.unlock();
            if (currentAmountOfUsers > lastAmountOfUsersSent + USERS_AMOUNT_CHANGED_TO_INFORM_PROXY || currentAmountOfUsers < lastAmountOfUsersSent - USERS_AMOUNT_CHANGED_TO_INFORM_PROXY)
            {
                //send proxy current amount
                unsigned char* buffer = JsonPacketSerialization::createUpdateUsersResult(currentAmountOfUsers);
                if(send(this->m_proxySocket, buffer, BUFFER_SIZE, 0) == -1){
                    std::cout << "error with proxy server connection" << std::endl;
                }
                delete buffer;


                // update parameter for the next loop
                lastAmountOfUsersSent = currentAmountOfUsers;
            }
            start = std::chrono::high_resolution_clock::now();;
        }
    }
}

