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
#include "RSAHandler.hpp"

class ThreadPool;
class RequestHandlerFactory;

#define PORT 8080
#define DATABASE_SERVER_PORT 5000
#define DATABASE_IP_ADDRESS "127.0.0.1"
#define THREAD_POOL_SIZE 10
#define THREADS_COUNT 1

class Server{
    public:
        Server();
        ~Server();

        void run();
        void shutdown();
        static bool handleClient(int, Server*);

    private:
        void bindAndListen();
        //void handleNewClient(int clientSocket);
        void serverThread(std::thread*);

        void sendClientsToThreads();
        int connectToDatabaseServer();
        static void removeClient(int, Server*);

        int m_serverSocket;
        int m_databaseServerSocket;
        
        std::map<int, ClientData*> m_clients;
        RequestHandlerFactory* m_requestHandlerFactory;
        ThreadPool* m_threadPool;

};