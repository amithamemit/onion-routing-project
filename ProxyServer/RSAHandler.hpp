#pragma once
#include <iostream>
#include "IRequestHandler.hpp"
#include "RSA_Communication.hpp"
#include "RequestHandlerFactory.hpp"
#include "SSHConnection.hpp"

class DatabaseServerCommunication;

#define IS_NODE_CODE 1

class RSAHandler{
    public:
        RSAHandler();
        ~RSAHandler();

        static RSA_Communication* startRSAConnversation(int, std::vector<RelayServer>*, DatabaseServerCommunication*);
};