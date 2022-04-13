#include "RSAHandler.hpp"

RSAHandler::RSAHandler(){
}

RSAHandler::~RSAHandler(){

}

/*
Method that start RSA connversation with client.
First, the client send to the server his public key.
After that the server send to the client his public key.
this method return RSA_Communication object which after the key transfer can decrypy and encrypt messages.
Input: client socket.
Output: RSA_Commuication objcet.
*/
RSA_Communication* RSAHandler::startRSAConnversation(int clientSocket, std::vector<RelayServer>* relayServers, DatabaseServerCommunication* dbConnection){
    unsigned char buffer[BUFFER_SIZE] = {0};
    RSA_Communication* RSACommuincation = nullptr;
    // getting the client first request. this request will have the client public key.
    if(read(clientSocket , buffer, BUFFER_SIZE) <= 0){
        std::cout << Helper::getCurrentTime() << "Error with read message from client. Client disconnected..." << std::endl;
        return nullptr;
    }

    if(buffer[0] == IS_NODE_CODE){
        if(SSHConnection::isNode(clientSocket, dbConnection, JsonPacketDeserialization::getSocketData(buffer))){
            RelayServer newRelayServer = {};
            newRelayServer.connectedSocket = clientSocket;
            newRelayServer.numberOfConnectedUsers = 0;
            relayServers->push_back(newRelayServer);
        }
    }

    // getting the public key from client request
    std::string clientPublicKey = JsonPacketDeserialization::getPublicKey(buffer);

    // create rsa communication object that encrypt/decrypt using rsa keys
    RSACommuincation = new RSA_Communication(clientPublicKey);

    // send to client the proxy server public key
    std::string proxyPublicKey = RSACommuincation->getMyPublicKey();
    unsigned char* response = JsonPacketSerialization::createPublicKeyResponse(proxyPublicKey);
    if(send(clientSocket , response, BUFFER_SIZE, 0) == -1){
        std::cout << Helper::getCurrentTime() << "Error with send message to client. Client disconnected..." << std::endl;
        delete RSACommuincation;
        return nullptr;
    }
    return RSACommuincation;
}