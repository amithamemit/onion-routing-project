#pragma once
#include "IRequestHandler.hpp"
#include "RSA_Communication.hpp"
#include <vector>

class IRequestHandler;

#define BUFFER_SIZE 256

struct RequestResult{
    IRequestHandler* newHandler;
    unsigned char* buffer;    
};

struct RequestInfo{
    unsigned char* buffer;
};

struct LoginInfo {
	std::string username;
	std::string password;
};

struct LoginResult {
	int code;
	int userId;
};

struct SignupInfo {
	std::string username;
	std::string password;
};

struct SignupResult {
	int code;
};

struct ErrorResult {
	std::string errorMessage;
	int code;
};

struct LoggedUser{
	int userId;
	std::string username;
};

struct RelayServer{
	int connectedSocket;
	int numberOfConnectedUsers;
};

struct ClientData{
	IRequestHandler* currentHandler;
	RSA_Communication* RSACommunication;
};

struct SocketData{
	std::string ipAddress;
	int port;
};

struct relayInfo
{
	std::string ipAdress;
	int port;
};

struct PathRequest
{
    int relaysAmount;
    std::string lastRelayCountry;
};


struct avaliableLastNodes
{
	std::vector<std::string> _countries; //only relay location will be avaliable to client, no need to send ip and port 
};

struct lastRelayRequest
{
	std::string relayLocation;
};

struct nextNode{
	int port;
	std::string ipAdress;
	std::string encryptKey;
};

struct relaysPath //response for path request
{
    std::vector<nextNode> _path;
    std::string serverResponse;
};

enum codes{
	loginCode = 0,
	signupCode = 1,

	accessApprovedCode = 1,
	wrongPasswordCode = 2,
	wrongUsernameCode = 3,
	signupSucceed = 4,
	usernameExists = 5,


	createPath = 20,
	createPathResponse = 21,

	getLastRelays = 25,
	LastRelaysResponse = 26,

	
	weekPassword = 90,
	userAlreadyLoggedInCode = 91,

	databaseServerError = 100

};

enum databaseServerCodes{
	databaseServerLoginCode = 1,
	databaseServerSingupCode = 2
};