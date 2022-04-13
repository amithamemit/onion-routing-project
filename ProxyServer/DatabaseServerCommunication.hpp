#pragma once
#include <iostream>
#include "Server.hpp"
#include "json.hpp"

using nlohmann::json;

class DatabaseServerCommunication{
    public:
        DatabaseServerCommunication(int);
        ~DatabaseServerCommunication();

        unsigned char* sendRequest(int code, json j);

    private:
        int m_databaseServerSocket;
};