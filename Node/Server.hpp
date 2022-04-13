#pragma once
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
#include "IRequestHandler.hpp"
#include "RequestHandlerFactory.hpp"
#include "ThreadPool.hpp"
#include "JsonPacketDeserialization.hpp"

class ThreadPool;
class RequestHandlerFactory;

#define DATABASE_SERVER_PORT 5000
#define DATABASE_IP_ADDRESS "127.0.0.1"
#define PROXY_SERVER_PORT 8080
#define PROXY_SERVER_ADDRESS "127.0.0.1"
#define THREAD_POOL_SIZE 10
#define THREADS_COUNT 1
#define USERS_AMOUNT_CHANGED_TO_INFORM_PROXY 2

class Server{
    friend class ThreadPool;

    public:
        Server(int);
        ~Server();

        void run();
        void shutdown();
        //static bool handleClient(int, Server*);

    private:
        void bindAndListen();
        //void handleNewClient(int clientSocket);
        void communicationWithProxy();
        void serverThread(std::thread*);
        void sendClientsToThreads();
        void createPathThread();
        CommunicationData buildPath(int clientSocket);
        int connectToDatabaseServer();
        int connectToProxyServer();

    
        static void removeClient(int, Server*);

        static bool handleRequest(CommunicationData, Server*);

        int m_port; //this relay's port
        int m_serverSocket; //this relay's listening socket
        int m_proxySocket; //proxy server's socket
        int m_databaseServerSocket;//socket with database server
        ThreadPool* m_threadPool; //class to handle thread pool
        std::queue<CommunicationData> m_conversations; //existing convesations to run on thread pool
        std::mutex m_conversationsLock;//lock for existing convesations to run on thread pool
        std::queue<int> m_socketsToStartConvesrations; //new sockets waiting for path creation queue
        std::mutex m_NewSocketLock; //lock for new sockets waiting for path creation queue
        RSA_Communication* m_serverDecryptRSA; //conversation encrypt and decrypt handler
};