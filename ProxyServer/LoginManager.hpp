#pragma once
#include <iostream>
#include "DataStructures.hpp"
#include "DatabaseServerCommunication.hpp"
#include "json.hpp"

using nlohmann::json;

class DatabaseServerCommunication;

class LoginManager{
    public:
        LoginManager(DatabaseServerCommunication*);
        ~LoginManager();

        LoginResult login(std::string, std::string);
        SignupResult signup(std::string, std::string);

    private:
        DatabaseServerCommunication* m_databaseServerCommunication;

        unsigned char* createBufferToDatabaseServer(int code, json j);
};