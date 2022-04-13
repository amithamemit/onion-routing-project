#include "Server.hpp"


/*
server constructor
*/
Server::Server(){
    RSA_Communication::generateKeys();
    this->m_serverSocket = 0;
    this->m_databaseServerSocket = this->connectToDatabaseServer();
    this->m_clients = std::map<int, ClientData*>();
    this->m_requestHandlerFactory = new RequestHandlerFactory(this->m_databaseServerSocket);
    this->m_threadPool = new ThreadPool(THREADS_COUNT, this);
}

/*
server destructor
*/
Server::~Server(){
    delete this->m_requestHandlerFactory;
    delete this->m_threadPool;
}

/*
method that start the server.
Input: none.
Output: none.
*/
void Server::run(){
    std::thread bindAndListenThread = std::thread(&Server::bindAndListen, this);
    bindAndListenThread.detach();
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
    std::cout << "Closing database server socket..." << std::endl;
    close(this->m_databaseServerSocket);
    for(const auto& clientSocket : this->m_clients){
        ::shutdown(clientSocket.first, SHUT_RDWR);
        std::cout << "Closing client socket number " << clientSocket.first << std::endl;
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
    address.sin_port = htons( PORT );
       
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
    std::cout << Helper::getCurrentTime() << "Listening on port " << PORT << "..." << std::endl;

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

            // transfer keys and get the RSA_Communication object to encrypt/decrypt messages.
            RSA_Communication* RSACommunication = RSAHandler::startRSAConnversation(new_socket, this->m_requestHandlerFactory->getRelayServers(), this->m_requestHandlerFactory->getDatabaseServerCommunication());
            ClientData* clientData = new ClientData;
            clientData->RSACommunication = RSACommunication;
            clientData->currentHandler = this->m_requestHandlerFactory->createLoginRequestHandler(RSACommunication);
            this->m_clients[new_socket] = clientData;
            this->m_threadPool->addTask(new_socket);
        }
    }
}

/*
Method that handle client request if there is bytes to read in the socket.
The thread pool check if there are bytes to read.
If there are bytes to read the handleClient is called and start to run.
Input: client socket to handle, and server pointer to access private data (because the method is static).
Output: if the client still connected true, else false.
*/
bool Server::handleClient(int clientSocket, Server* server){
    bool clientConnected = true;
    unsigned char buffer[BUFFER_SIZE] = {0};
    /*if(server->m_clients.find(clientSocket) == server->m_clients.end()){
        server->m_clients[clientSocket]->currentHandler = server->m_requestHandlerFactory->createLoginRequestHandler(server->m_clients[clientSocket]->RSACommunication);
    }*/

    // if client disconnected
    if(read(clientSocket , buffer, BUFFER_SIZE) <= 0){
        Server::removeClient(clientSocket, server);
        std::cout << Helper::getCurrentTime() << "Error with read message from client. Client disconnected..." << std::endl;
        clientConnected = false;
    }

    // create request info
    RequestInfo requestInfo = {};
    requestInfo.buffer = buffer;

    std::cout << "handle request" << std::endl;
    // getting request result
    RequestResult requestResult = server->m_clients[clientSocket]->currentHandler->handleRequest(requestInfo);

    // if the user change handler need to delete the old handler
    if (requestResult.newHandler != server->m_clients[clientSocket]->currentHandler){
        delete server->m_clients[clientSocket]; // delete the old handler
        server->m_clients[clientSocket]->currentHandler = requestResult.newHandler; // change the current user handler
    }

    // send response to client
    if(send(clientSocket , requestResult.buffer, BUFFER_SIZE, 0) == -1){
        Server::removeClient(clientSocket, server);
        std::cout << Helper::getCurrentTime() << "Error with send message to client. Client disconnected..." << std::endl;
        clientConnected = false;
    }
    delete requestResult.buffer;
    return clientConnected;
}

void Server::removeClient(int clientSocket, Server* server){
    delete server->m_clients[clientSocket];
    server->m_clients.erase(clientSocket);
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
        std::cout << Helper::getCurrentTime() << "Invalid address/ Address not supported" << std::endl;;
        return -1;
    }
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cout << Helper::getCurrentTime() << "Connection failed to database server" << std::endl;;
        return -1;
    }
    std::cout << Helper::getCurrentTime() << "Connection to database server succeed" << std::endl;
    return sock;
}