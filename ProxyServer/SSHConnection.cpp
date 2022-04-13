#include "SSHConnection.hpp"

bool SSHConnection::isNode(int sock, DatabaseServerCommunication* dbConnection, SocketData socketData){
    // request the public key of the relay server from the database.
    std::string relayServerPublicKey = "";
    // getting the public key from the datbase


    if(relayServerPublicKey == ""){
        return false;
    }


    // create a random string
    std::string randomString = createRandomString(2048);
    
    // encrypt the string using the relay server public key
    RSA_Communication* rsa = new RSA_Communication(relayServerPublicKey);
    unsigned char* encryptedString = rsa->encrypt((unsigned char*)randomString.c_str());

    // send the encrypted string to the node


    // getting the node response
    std::string nodeDecryptedRandomString = "";

    // if the decrypted random string and the random string equal, thats means that the node have the correct private key.
    return nodeDecryptedRandomString == randomString;
}

std::string SSHConnection::createRandomString(int length){
    std::string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    int i = 0;
    while(str.length() != length){
        str += str[i];
        i++;
        if(i == str.length()){
            i = 0;
        }
    }

     std::random_device rd;
     std::mt19937 generator(rd());

     std::shuffle(str.begin(), str.end(), generator);

     return str.substr(0, length);    // assumes 32 < number of characters in str
}