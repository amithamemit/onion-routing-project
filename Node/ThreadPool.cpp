#include "ThreadPool.hpp"

/*
* defualt constructor for thread pool
* input: number of threads to have in the thread pool and the server that uses the thread pool
* output: none
*/
ThreadPool::ThreadPool(int numberOfThreads, Server* server){
    // create n threads
    for(int i = 0; i < numberOfThreads; i++){
        std::thread newThread = std::thread(&ThreadPool::threadLoop, this);
        newThread.detach();
    }
    this->m_server = server;
}

/*
* defulat destructor for thread pool
*/
ThreadPool::~ThreadPool(){

}


/*
this function is the functions that runs on the threads in the thread pool, this function checks if there is a task waiting in the queue (converation queue and a task is a message waiting in the socket)
if there is a task in the queue the thread does the task and return the task (converation) to the queue.
Input: none
Output: None
*/
void ThreadPool::threadLoop(){
    while(true){
        // lock the queue mutex
        this->m_server->m_conversationsLock.lock();

        //check if there is a conversation in queue
        if(this->m_server->m_conversations.empty()){
            this->m_server->m_conversationsLock.unlock();
            continue;
        }

        // get client socket and remove it from the queue
        CommunicationData conversationData = this->m_server->m_conversations.front();
        bool clientConnected = true;
        this->m_server->m_conversations.pop();

        // unlock the queue mutex
        this->m_server->m_conversationsLock.unlock();

        // check if conversation has a message in buffer
        int bytes_available_in_first_socket = 0;
        int bytes_available_in_second_socket = 0;

        ioctl(conversationData.socketToDecrypt, FIONREAD, &bytes_available_in_first_socket);
        ioctl(conversationData.socketToEncrypt, FIONREAD, &bytes_available_in_second_socket);
        
        if(bytes_available_in_second_socket != 0 || bytes_available_in_first_socket != 0){
            clientConnected = Server::handleRequest(conversationData, this->m_server);
        }

        if(clientConnected){
            // insert the converstation back to the queue
            this->m_server->m_conversationsLock.lock();
            this->m_server->m_conversations.push(conversationData);
            this->m_server->m_conversationsLock.unlock();
        }
    }
}
