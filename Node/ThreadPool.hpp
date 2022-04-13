#pragma once
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include "Server.hpp"

class Server;
struct CommunicationData;
struct RelayServer;

class ThreadPool{
    public:
        ThreadPool(int numberOfThreads, Server*);
        ~ThreadPool();

    private:
        void threadLoop();
        Server* m_server; //server that uses the thread pool
};