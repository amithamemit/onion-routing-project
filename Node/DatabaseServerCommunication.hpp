#pragma once
#include <iostream>
#include "Server.hpp"
#include "json.hpp"
#include "Helper.hpp"

using nlohmann::json;

class DatabaseServerCommunication{
    public:
        DatabaseServerCommunication(int);
        ~DatabaseServerCommunication();

        unsigned char* sendRequest(int code, json j);

    private:
        int m_databaseServerSocket; //the socket of the conversation with the database server
};