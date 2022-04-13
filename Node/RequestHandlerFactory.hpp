#pragma once
#include <iostream>
#include <vector>
#include <mutex>
#include "IRequestHandler.hpp"
#include "DatabaseServerCommunication.hpp"

class DatabaseServerCommunication;
class LoginManager;

class RequestHandlerFactory{
    public:
        RequestHandlerFactory(int);
        ~RequestHandlerFactory();

        IRequestHandler* createLoginRequestHandler();

    private:

        void refreshRelayServers();
        static bool compareRelaysByConnectedUsers(const RelayServer& first, const RelayServer& second);

        DatabaseServerCommunication* m_databaseServerCommunication; //class to communicate with database server
};