#include "ThreadPool.hpp"

ThreadPool::ThreadPool(int numberOfThreads, Server* server){
    // create n threads
    for(int i = 0; i < numberOfThreads; i++){
        std::thread newThread = std::thread(&ThreadPool::threadLoop, this);
        newThread.detach();
    }
    this->m_server = server;
}

ThreadPool::~ThreadPool(){

}

/*
Method that add a new value for the thread pool.
This value will pass to an empty thread, and the thread with do the task with the passed value.
Input: template that have the values for the task
Output: None
*/
void ThreadPool::addTask(int clientSocket){
    // lock the mutex and add to the queue new socket
    this->m_queueMutex.lock();
    this->m_tasksQueue.push(clientSocket);
    this->m_queueMutex.unlock();
}

/*
Method that always get task, handle it with function and return the task to the thread pool queue.
Input: the function that the thread need to do.
Output: None
*/
void ThreadPool::threadLoop(){
    while(true){
        // lock the queue mutex
        this->m_queueMutex.lock();

        //check if there is a socket in queue
        if(this->m_tasksQueue.empty()){
            this->m_queueMutex.unlock();
            continue;
        }

        // get client socket and remove it from the queue
        int clientSocket = this->m_tasksQueue.front();
        bool clientConnected = true;
        this->m_tasksQueue.pop();

        // unlock the queue mutex
        this->m_queueMutex.unlock();

        // check if client sent message
        int bytes_available = 0;
        ioctl(clientSocket, FIONREAD, &bytes_available);
        
        if(bytes_available != 0){
            clientConnected = Server::handleClient(clientSocket, this->m_server);
        }

        if(clientConnected){
            // insert the socket back to the queue
            this->m_queueMutex.lock();
            this->m_tasksQueue.push(clientSocket);
            this->m_queueMutex.unlock();
        }
    }
}
