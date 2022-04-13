#pragma once
#include <iostream>
#include "RequestHandlerFactory.hpp"
#include <string>
#include <random>

class DatabaseServerCommunication;

class SSHConnection{
public:
    static bool isNode(int, DatabaseServerCommunication*, SocketData);
    static std::string createRandomString(int);
private:
    std::string getNodePublicKey(int);

    RSA_Communication* m_rsa;
};
