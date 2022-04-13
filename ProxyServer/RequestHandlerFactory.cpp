#include "RequestHandlerFactory.hpp"

RequestHandlerFactory::RequestHandlerFactory(int databaseServerSocket){
    this->m_databaseServerCommunication = new DatabaseServerCommunication(databaseServerSocket);
    this->m_loginManager = new LoginManager(this->m_databaseServerCommunication);

    //releasing the thread to refresh the vector of relay servers
    //std::thread newThread = std::thread(&RequestHandlerFactory::refreshRelayServers, this);
    //newThread.detach();
}

RequestHandlerFactory::~RequestHandlerFactory(){
    delete this->m_loginManager;
    delete this->m_databaseServerCommunication;
}

IRequestHandler* RequestHandlerFactory::createLoginRequestHandler(RSA_Communication* rsa){
    IRequestHandler* newHandler = new LoginRequestHandler(this->m_loginManager, this, rsa);
    return newHandler;
}

/*
function to run as thread and refresh the Relay Server list every 5 seconds
input: None
output: None
*/
void RequestHandlerFactory::refreshRelayServers(){
    while(true)
    {
        sleep(5);

        this->m_relayServersMutex.lock();
        int length = (*this->m_relayServers).size();
        this->m_relayServersMutex.unlock();
        for (int i = 0; i < length; i++)
        {
            this->m_relayServersMutex.lock();
            // check if update message is waiting in socket
            int bytes_available = 0;
            ioctl(this->m_relayServers->at(i).connectedSocket, FIONREAD, &bytes_available);
        
            if(bytes_available != 0){
                //get the amount and update relayServers
                unsigned char buffer[BUFFER_SIZE] = {0};

                // if relay disconnected remove it from the vector and print message.
                if(read(this->m_relayServers->at(i).connectedSocket , buffer, BUFFER_SIZE) <= 0){
                    std::cout << Helper::getCurrentTime() << "Node disconnected." << std::endl;
                    this->m_relayServers->erase(this->m_relayServers->begin() + i);
                }

                std::cout << "calling to method." << std::endl;
                // update the connected users count
                int connectedUsers = JsonPacketDeserialization::getConnectUsersCount(buffer);
                this->m_relayServers->at(i).numberOfConnectedUsers = connectedUsers;
                std::cout << Helper::getCurrentTime() << "Got update from node " << i << ": " << connectedUsers << " use this node now." << std::endl;
                this->m_relayServersMutex.unlock();
            }
        }
    }
}

/*
Static method that compare between 2 releys by their connected users.
Input: first relay and the second relay to compare.
Output: true if the first relay have less connected users, else false.
*/
bool RequestHandlerFactory::compareRelaysByConnectedUsers(const RelayServer& first, const RelayServer& second){
    return first.numberOfConnectedUsers < second.numberOfConnectedUsers;
}

std::vector<RelayServer>* RequestHandlerFactory::getRelayServers(){
    return this->m_relayServers;
}

DatabaseServerCommunication* RequestHandlerFactory::getDatabaseServerCommunication(){
    return this->m_databaseServerCommunication;
}