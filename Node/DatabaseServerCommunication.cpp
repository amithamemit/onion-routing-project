#include "DatabaseServerCommunication.hpp"

/*
* default constructor for databaseServerCommunication class
* input: database server's socket
* output: none
*/
DatabaseServerCommunication::DatabaseServerCommunication(int serverSocket){
    this->m_databaseServerSocket = serverSocket;
}

DatabaseServerCommunication::~DatabaseServerCommunication(){

}

/*
*this function is creating the message according to the format (message id +  json content)
* input: code id and json to send
* output:the response from the database server
*/
unsigned char* DatabaseServerCommunication::sendRequest(int code, json j){
    // creating buffer in next format: 1 byte for the code, and after the json string
    std::string jsonString = j.dump(); //this variable is the text representation of the kson message
    unsigned char* buffer = new unsigned char[1 + jsonString.length()]; //this variable is the message to send (message id + json)
    memset(buffer, 0, sizeof(buffer)); //reset buffer

    // adding the code to the buffer
    buffer[0] = code;
    // adding the json string to the buffer
    for(int i = 0; i < jsonString.length(); i++){
        buffer[i + 1] = jsonString[i];
    }


    unsigned char* responseBuffer = new unsigned char[BUFFER_SIZE]; //the buffer to store server response

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