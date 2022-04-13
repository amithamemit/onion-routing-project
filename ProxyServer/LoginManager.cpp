#include "LoginManager.hpp"

LoginManager::LoginManager(DatabaseServerCommunication* databaseServerCommunication){
    this->m_databaseServerCommunication = databaseServerCommunication;
}

LoginManager::~LoginManager(){
    
}

LoginResult LoginManager::login(std::string username, std::string password){
    json j;
    j["username"] = username;
    j["password"] = password;
    LoginResult loginResult = {};

   // send request to the database server and get response
    unsigned char* buffer = this->m_databaseServerCommunication->sendRequest(databaseServerLoginCode, j);

    // check if there is database server error. if the buffer is nullptr, there is server error.
    if (!buffer){
        loginResult.code = databaseServerError;
        loginResult.userId = 0;
    }
    else{
        // getting the login result from the database server resopnse
        loginResult = JsonPacketDeserialization::getDatabaseLoginResult(buffer, nullptr);
        delete buffer; // clean memory
    }
    return loginResult;
}

SignupResult LoginManager::signup(std::string username, std::string password){
    json j;
    j["username"] = username;
    j["password"] = password;

   // send request to the database server and get response
    unsigned char* buffer = this->m_databaseServerCommunication->sendRequest(databaseServerSingupCode, j);
    // getting the login result from the database server resopnse
    SignupResult signupResult = JsonPacketDeserialization::getDatabaseSignupResult(buffer, nullptr);
    delete buffer; // clean memory
    return signupResult;
}