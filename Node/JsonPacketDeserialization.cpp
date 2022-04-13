#include "JsonPacketDeserialization.hpp"

/*
MESSAGE FORMAT: MESSAGE LENGTH(4 bytes) JSON MESSAGE(n bytes)
*/

/*
* this function gets the login variables from the json and puts them in a struct
* input: the json written in text
* output: login info in a struct
*/
LoginInfo JsonPacketDeserialization::getLoginInfo(unsigned char* buffer){
    json j = JsonPacketDeserialization::getJsonFromBuffer(buffer); //json that contains login request variables

    LoginInfo loginInfo; //login info struct to load variables into
    loginInfo.password = j["password"];
    loginInfo.username = j["username"];

    return loginInfo;
}

/*
* this function gets the next relay variables from the json and puts them in a struct
* input: the json written in text
* output: next relay info in a struct
*/
nextNode JsonPacketDeserialization::getNextNode(unsigned char* buffer)
{
    json j = JsonPacketDeserialization::getJsonFromBuffer(buffer); //json that contains next relay variables

    nextNode nextRelayJson; //struct that saves the neccesary info of the next relay
    nextRelayJson.ipAdress = j["ip_adress"];
    nextRelayJson.port = j["port"];
    nextRelayJson.encryptKey = j["encrypt_key"];

    return nextRelayJson;
}

/*
* this function gets the signup variables from the json and puts them in a struct
* input: the json written in text
* output: signup info in a struct
*/
SignupInfo JsonPacketDeserialization::getSignupInfo(unsigned char* buffer){
    json j = JsonPacketDeserialization::getJsonFromBuffer(buffer); //json that contains signup request variables

    //create signup info struct
    SignupInfo signupInfo;
    signupInfo.password = j["password"];
    signupInfo.username = j["username"];

    return signupInfo;
}

/*
* this function gets the databsae login response variables from the json and puts them in a struct
* input: the json written in text
* output: login response info in a struct
*/
LoginResult JsonPacketDeserialization::getDatabaseLoginResult(unsigned char* buffer){
    json j = JsonPacketDeserialization::getJsonFromDatabaseServerBuffer(buffer); //json that contains the login response variables

    // create login result struct
    LoginResult loginResult;
    loginResult.code = j["code"];
    loginResult.userId = j["user_id"];

    return loginResult;
}

/*
* this function gets the database server's signup response variables from the json and puts them in a struct
* input: the json written in text
* output: signup response info in a struct
*/
SignupResult JsonPacketDeserialization::getDatabaseSignupResult(unsigned char* buffer){
    json j = JsonPacketDeserialization::getJsonFromDatabaseServerBuffer(buffer); //json that contains the signup response variables
    // create signup result struct
    SignupResult signupResult;
    signupResult.code = j["code"];

    return signupResult;
}



/*
function that returns the message length in decimal
input: the message length in hex
output: the length in decimal
*/
int JsonPacketDeserialization::getMessageLength(unsigned char* buffer){
    return Helper::convert4ByteToInt(buffer);
}

/*
function that get from buffer the json message and return it
Input: buffer that contains the json
Output: json message from the buffer in the json format
*/
json JsonPacketDeserialization::getJsonFromBuffer(unsigned char* buffer){
    //getting message length
    int messageLength = JsonPacketDeserialization::getMessageLength(buffer); //the message length in decimal
    std::string message = ""; //space to save the json represented in text from the array
    //copy the message from the buffer to new string
    for (int i = BYTES_LEN + 1; i < messageLength + BYTES_LEN + 1; i++) {
        message += buffer[i];
    }
    //convert the string to a json foramt
    json j = json::parse(message); //the json that the buffer represented
    return j;
}


/*
Method that get the json object from database server buffer.
Input: database server buffer.
Output: json in the buffer.
*/
json JsonPacketDeserialization::getJsonFromDatabaseServerBuffer(unsigned char* buffer){
    //getting message length
    int messageLength = JsonPacketDeserialization::getMessageLength(buffer); //message length in decimal
    std::string message = ""; //sapce to copy the json representation from the buffer array
    //copy the message from the buffer to new string
    for (int i = BYTES_LEN; i < messageLength + BYTES_LEN + 1; i++) {
        message += buffer[i];
    }
    //convert the string to a json foramt
    json j = json::parse(message);//the json that the buffer represents
    return j;
}