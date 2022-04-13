#include "DatabaseServerCommunication.hpp"

DatabaseServerCommunication::DatabaseServerCommunication(int serverSocket){
    this->m_databaseServerSocket = serverSocket;
}

DatabaseServerCommunication::~DatabaseServerCommunication(){

}

unsigned char* DatabaseServerCommunication::sendRequest(int code, json j){
    // creating buffer in next format: 1 byte for the code, and after the json string
    std::string jsonString = j.dump();
    unsigned char* buffer = new unsigned char[1 + jsonString.length()];
    memset(buffer, 0, sizeof(buffer)); //reset buffer

    // adding the code to the buffer
    buffer[0] = code;
    // adding the json string to the buffer
    for(int i = 0; i < jsonString.length(); i++){
        buffer[i + 1] = jsonString[i];
    }


    unsigned char* responseBuffer = new unsigned char[BUFFER_SIZE];

    // sending to the database server request
    if(send(this->m_databaseServerSocket , buffer, 1 + jsonString.length(), 0) == -1){
        std::cout << Helper::getCurrentTime() << "Cant send message for database server. Error with database server connection..." << std::endl;
        return nullptr;
    }

    delete buffer; // clean memory

    // if client disconnected
    if(read(this->m_databaseServerSocket , responseBuffer, BUFFER_SIZE) <= 0){
        std::cout << Helper::getCurrentTime() << "Cant read message from database server. Error with database server connection..." << std::endl;
        return nullptr;
    }
    return responseBuffer;
}