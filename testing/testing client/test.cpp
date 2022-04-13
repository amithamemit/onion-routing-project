
#include <iostream>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <map>
#include <thread>
#include <unistd.h>
#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <chrono>
//#include "JsonPacketSerialization.hpp"
#include "Helper.hpp"
#include "json.hpp"
#include "DataStructures.hpp"
#include "RSA_Communication.hpp"


unsigned char* getJsonBufferRequest(json j, int code);
unsigned char* createNextNodeResult(nextNode nextRelay);
unsigned char* changeIntTo4ByteCharArray2(int num);
unsigned char* encryptNtimes(int n, RSA_Communication** rsaArr, unsigned char* buffer);
int main()
{

    RSA_Communication* nodesRsaArr[3];
    for(int i = 0; i < 3; i++){
        nodesRsaArr[i] = new RSA_Communication("", i);
    }


    int newSocket = 0;
    struct sockaddr_in serv_addr;
    if ((newSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "Socket creation error for next relay." << std::endl;
        
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(100);
       
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <=0 )
    {
        std::cout << "socket creation error" << std::endl;
        return 0;
    }
    std::cout << "Connecting to the first node..." << std::endl;
    sleep(2);
    if (connect(newSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cout << "socket creation error" << std::endl;
        return 0;
    }

    nextNode nextDestination;

    std::cout << "Connecting to the second node..." << std::endl;
    nextDestination.encryptKey = nodesRsaArr[0]->getMyPublicKey();
    nextDestination.ipAdress = "127.0.0.1";
    nextDestination.port = 101;
    unsigned char* buffer = createNextNodeResult(nextDestination);
    sleep(2);
    if(send(newSocket, buffer, 1024, 0) == -1)
    { //changed from decrypted ro toDEcryptBuffer
        close(newSocket);
        return false;
    }
    delete buffer;

    std::cout << "Connecting to the third node..." << std::endl;
    nextDestination.encryptKey = nodesRsaArr[1]->getMyPublicKey();
    nextDestination.port = 102;
    buffer = createNextNodeResult(nextDestination);
    //unsigned char* toDelete = buffer;
    //buffer = encryptNtimes(1, nodesRsaArr, buffer);
    //delete toDelete;
    sleep(2);
    if(send(newSocket, buffer, 1024, 0) == -1)
    { //changed from decrypted ro toDEcryptBuffer
        close(newSocket);
        return false;
    }
    delete buffer;

    nextDestination.encryptKey = nodesRsaArr[2]->getMyPublicKey();
    nextDestination.port = 200;
    buffer = createNextNodeResult(nextDestination);
    //toDelete = buffer;
    //buffer = encryptNtimes(2, nodesRsaArr, buffer);
    //delete toDelete;
    std::cout << "connecting to test server" << std::endl;
    sleep(2);
    if(send(newSocket, buffer, 1024, 0) == -1)
    { //changed from decrypted ro toDEcryptBuffer
        close(newSocket);
        return false;
    }
    delete buffer;

    //buffer = encryptNtimes(3, nodesRsaArr, (unsigned char*)"hi");

    std::cout << "sending hi to server..." << std::endl;
    sleep(2);
    if(send(newSocket, "hi", 1024, 0) == -1)
    { //changed from decrypted ro toDEcryptBuffer
        close(newSocket);
        return false;
    }
    //delete buffer;

    buffer = new unsigned char[1024];
    memset(buffer, 0, 1024);

    sleep(2);
    if(read(newSocket , buffer, 1024) <= 0){
        close(newSocket);
        std::cout << "Error with read message from client. Client disconnected..." << std::endl;
    }

    unsigned char* decrypted = buffer;
    unsigned char* toClean;
    for(int i = 0; i < 3; i++){
        toClean = decrypted;
        decrypted = nodesRsaArr[i]->decrypt(decrypted);
        delete toClean;
    }

    std::cout << "got " << decrypted << " from server." << std::endl;

    delete decrypted;

    //clean memory
    for(int i = 0; i < 3; i++){
        delete nodesRsaArr[i];
    }

}

unsigned char* createNextNodeResult(nextNode nextRelay){
	json j;
	j["ip_adress"] = nextRelay.ipAdress;
    j["port"] = nextRelay.port;
    j["encrypt_key"] = nextRelay.encryptKey;

	return getJsonBufferRequest(j, 0);
}

unsigned char* getJsonBufferRequest(json j, int code)
{
	unsigned char* buffer;
	//getting json string
	std::string jsonString = j.dump();

	// create a buffer to the response
	buffer = new unsigned char[1 + BYTES_LEN + jsonString.length()];
    // add the code to the buffer
    buffer[0] = code;
	unsigned char* byteLen = Helper::changeIntTo4ByteCharArray(jsonString.length());
	//add the bytes to the buffer
	for (int i = 1; i < BYTES_LEN + 1; i++) {
		buffer[i] = byteLen[i-1];
	}
	delete byteLen; //clean memory

	//add the json string to the buffer
	for (int i = 0; i < jsonString.length(); i++) {
		buffer[i + BYTES_LEN + 1] = (unsigned char)jsonString[i];
	}
	return buffer;
}

unsigned char* changeIntTo4ByteCharArray2(int num){
	unsigned char* bytes = new unsigned char[4];
	//change the number to 4 byte format
	bytes[0] = (num >> 24) & 0xff;
	bytes[1] = (num >> 16) & 0xff;
	bytes[2] = (num >> 8) & 0xff;
	bytes[3] = num & 0xff;

	return bytes;
}


unsigned char* encryptNtimes(int n, RSA_Communication** rsa, unsigned char* buffer){
    if(n == 1){
        return rsa[0]->encrypt(buffer);
    }
    if(n == 2){
        unsigned char* encrypted1Time = rsa[1]->encrypt(buffer);
        unsigned char* encrypted2Times = rsa[0]->encrypt(encrypted1Time);
        delete encrypted1Time;
        return encrypted2Times;
    }
    if(n == 3){
        unsigned char* encrypted1Time = rsa[2]->encrypt(buffer);
        unsigned char* encrypted2Times = rsa[1]->encrypt(encrypted1Time);
        delete encrypted1Time;
        unsigned char* encrypted3Times = rsa[0]->encrypt(encrypted2Times);
        delete encrypted2Times;
        return encrypted3Times;
    }
    return nullptr;
}