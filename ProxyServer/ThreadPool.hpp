#pragma once
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include "Server.hpp"

class Server;

class ThreadPool{
    public:
        ThreadPool(int numberOfThreads, Server*);
        ~ThreadPool();
        void addTask(int);

    private:
        void threadLoop();
        std::queue<int> m_tasksQueue;
        std::mutex m_queueMutex;
        Server* m_server;
};