#include "JsonPacketDeserialization.hpp"


/*
MESSAGE FORMAT: MESSAGE LENGTH(4 bytes) JSON MESSAGE(n bytes)
*/

signupAndLoginResponse JsonPacketDeserialization::getLoginAndSignupResponse(unsigned char* encryptedBuffer, RSA_Communication* rsa)
{
    unsigned char* buffer = rsa->decrypt(encryptedBuffer);
    json j = JsonPacketDeserialization::getJsonFromBuffer(buffer);
    //create login info struct
    signupAndLoginResponse response;
    response.responseID = j["code"];

    // clean memory
    delete buffer;

    return response;
}

avaliableLastNodes JsonPacketDeserialization::avaliableLastRelaysResponse(unsigned char* encryptedBuffer, RSA_Communication* rsa)
{
    unsigned char* buffer = rsa->decrypt(encryptedBuffer);
    json j = JsonPacketDeserialization::getJsonFromBuffer(buffer);
    avaliableLastNodes response;

    std::vector<std::string> tempCountriesVector(begin(j["countries"]), end(j["countries"])); //turn array to vector
    response._countries = tempCountriesVector;
    // clean memory
    delete buffer;

    return response;
}

relaysPath JsonPacketDeserialization::getRouteResponse(unsigned char* encryptedBuffer, RSA_Communication* rsa)
{
    unsigned char* buffer = rsa->decrypt(encryptedBuffer);
    json j = JsonPacketDeserialization::getJsonFromBuffer(buffer);
    relaysPath response;

    std::vector<nextNode> tempPathVector; //temperory vector to push to relays vector in relays path

    std::vector<std::string> tempPathIpAdresses(begin(j["path"]["ipAdress"]), end(j["path"]["ipAdress"])); //temperory ip adrress vector to push to relays vector to push to relay path
    std::vector<int> tempPathPorts(begin(j["path"]["port"]), end(j["path"]["port"])); //temperory port vector to push to relays vector to push to relay path
    std::vector<std::string> tempPathEncryptKeys(begin(j["path"]["encryptKey"]), end(j["path"]["encryptKey"])); //temperory encrypt key vector to push to relays vector to push to relay path


    for (int i = 0; i <= tempPathPorts.size(); i++) //same size in all temp path variables
    {
        nextNode tempNextNode;
        
        tempNextNode.port = tempPathPorts[i];
        tempNextNode.ipAdress = tempPathIpAdresses[i];
        tempNextNode.encryptKey = tempPathEncryptKeys[i];

        tempPathVector.push_back(tempNextNode);
    }
    response._path = tempPathVector;
    response.serverResponse = j["server_response"];

    //clean memory
    delete buffer;

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