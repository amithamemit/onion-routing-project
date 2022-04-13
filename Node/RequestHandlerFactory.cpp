#include "RequestHandlerFactory.hpp"

/*
* defaukt constructor for handler factory
* input: database server's socket
* output: none
*/
RequestHandlerFactory::RequestHandlerFactory(int databaseServerSocket){
    this->m_databaseServerCommunication = new DatabaseServerCommunication(databaseServerSocket);
}

/*
* default constructor
*/
RequestHandlerFactory::~RequestHandlerFactory(){
    delete this->m_databaseServerCommunication;
}

/*
* this function creates a login request handler (hadnler to deal with login)
* input: none
* output: new handler - login request handler
*/
IRequestHandler* RequestHandlerFactory::createLoginRequestHandler(){
    IRequestHandler* newHandler = nullptr;
    return newHandler;
}