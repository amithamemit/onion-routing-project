#pragma once
#include <iostream>
#include <vector>
#include <mutex>
#include "IRequestHandler.hpp"
#include "LoginRequestHandler.hpp"
#include "LoginManager.hpp"
#include "RSA_Communication.hpp"

class DatabaseServerCommunication;
class LoginManager;

class RequestHandlerFactory{
    public:
        RequestHandlerFactory(int);
        ~RequestHandlerFactory();

        IRequestHandler* createLoginRequestHandler(RSA_Communication* rsa);
        std::vector<RelayServer>* getRelayServers();
        DatabaseServerCommunication* getDatabaseServerCommunication();
    private:

        void refreshRelayServers();
        static bool compareRelaysByConnectedUsers(const RelayServer& first, const RelayServer& second);

        std::vector<RelayServer>* m_relayServers;
        std::mutex m_relayServersMutex;
        DatabaseServerCommunication* m_databaseServerCommunication;
        LoginManager* m_loginManager;
};