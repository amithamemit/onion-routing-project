#include "JsonPacketDeserialization.hpp"

/*
MESSAGE FORMAT: MESSAGE LENGTH(4 bytes) JSON MESSAGE(n bytes)
*/


LoginInfo JsonPacketDeserialization::getLoginInfo(unsigned char* encryptedBuffer, RSA_Communication* rsa){
    std::cout << "getting login info..." << std::endl;
    unsigned char* buffer = rsa->decrypt(encryptedBuffer);
    json j = JsonPacketDeserialization::getJsonFromBuffer(buffer);
    //create login info struct
    LoginInfo loginInfo;
    loginInfo.password = j["password"];
    loginInfo.username = j["username"];

    // clean memory
    delete buffer;

    return loginInfo;
}

SignupInfo JsonPacketDeserialization::getSignupInfo(unsigned char* encryptedBuffer, RSA_Communication* rsa){
    unsigned char* buffer = rsa->decrypt(encryptedBuffer);
    json j = JsonPacketDeserialization::getJsonFromBuffer(buffer);
    //create signup info struct
    SignupInfo signupInfo;
    signupInfo.password = j["password"];
    signupInfo.username = j["username"];

    // clean memory
    delete buffer;

    return signupInfo;
}

LoginResult JsonPacketDeserialization::getDatabaseLoginResult(unsigned char* encryptedBuffer, RSA_Communication* rsa){
    unsigned char* buffer = rsa->decrypt(encryptedBuffer);
    json j = JsonPacketDeserialization::getJsonFromDatabaseServerBuffer(buffer);
    // create login result struct
    LoginResult loginResult;
    loginResult.code = j["code"];
    loginResult.userId = j["user_id"];

    // clean memory
    delete buffer;

    return loginResult;
}

SignupResult JsonPacketDeserialization::getDatabaseSignupResult(unsigned char* encryptedBuffer, RSA_Communication* rsa){
    unsigned char* buffer = rsa->decrypt(encryptedBuffer);
    json j = JsonPacketDeserialization::getJsonFromDatabaseServerBuffer(buffer);
    // create login result struct
    SignupResult signupResult;
    signupResult.code = j["code"];

    // clean memory
    delete buffer;

    return signupResult;
}

std::string JsonPacketDeserialization::getPublicKey(unsigned char* buffer){
    json j = JsonPacketDeserialization::getJsonFromBuffer(buffer);
    // getting the public key from the json object
    std::string publicKey = j["publicKey"];
    
    return publicKey;
}

SocketData JsonPacketDeserialization::getSocketData(unsigned char* buffer){
    json j = JsonPacketDeserialization::getJsonFromBuffer(buffer);
    SocketData socketData = {};
    socketData.ipAddress = j["ip_address"];
    socketData.port = j["port"];

    return socketData;
}

int JsonPacketDeserialization::getConnectUsersCount(unsigned char* buffer){
    //getting message length
    int messageLength = JsonPacketDeserialization::getMessageLength(buffer);
    std::string message = "";
    //copy the message from the buffer to new string
    for (int i = BYTES_LEN + 1; i < messageLength + BYTES_LEN + 1; i++) {
        message += buffer[i];
    }
    //convert the string to a json foramt
    json j = json::parse(message);

    int connectedUsers = j["connectedUsers"];
    return connectedUsers;
}


PathRequest JsonPacketDeserialization::getPathRequest(unsigned char* buffer)
{
    PathRequest response;
    json j = JsonPacketDeserialization::getJsonFromBuffer(buffer);
    response.relaysAmount = j["relaysAmount"];
	response.lastRelayCountry = j["lastNodeCountry"];

    return response;
}

/*
function that return message length
*/
int JsonPacketDeserialization::getMessageLength(unsigned char* buffer){
    return Helper::convert4ByteToInt(buffer);
}

/*
function that get from buffer the json message and return it
Input: the buffer
Output: json message from the buffer
*/
json JsonPacketDeserialization::getJsonFromBuffer(unsigned char* buffer){
    //getting message length
    int messageLength = JsonPacketDeserialization::getMessageLength(buffer);
    std::string message = "";
    //copy the message from the buffer to new string
    for (int i = BYTES_LEN + 1; i < messageLength + BYTES_LEN + 1; i++) {
        message += buffer[i];
    }
    //convert the string to a json foramt
    json j = json::parse(message);
    return j;
}



/*
Method that get the json object from database server buffer.
Input: database server buffer.
Output: json in the buffer.
*/
json JsonPacketDeserialization::getJsonFromDatabaseServerBuffer(unsigned char* buffer){
    //getting message length
    int messageLength = JsonPacketDeserialization::getMessageLength(buffer);
    std::string message = "";
    //copy the message from the buffer to new string
    for (int i = BYTES_LEN; i < messageLength + BYTES_LEN + 1; i++) {
        message += buffer[i];
    }
    //convert the string to a json foramt
    json j = json::parse(message);
    return j;
}