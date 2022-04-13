#pragma once
#include "IRequestHandler.hpp"
#include "RSA_Communication.hpp"

class IRequestHandler;

#define BUFFER_SIZE 1024

struct RequestResult{
    IRequestHandler* newHandler; //the new handler 
    unsigned char* buffer; //the result of the request    
};

struct RequestInfo{
    unsigned char* buffer; //the request
};

struct LoginInfo {
	std::string username; //name of the user
	std::string password; //password of the user
};

struct LoginResult {
	int code; //the response code (logged in or not)
	int userId; //the id of the connected user
};

struct SignupInfo {
	std::string username; //the username of the new user
	std::string password; //the password of the new user
};

struct SignupResult {
	int code; //the response of the server (able to signup or not)
};

struct ErrorResult {
	std::string errorMessage; //the error message in text
	int code; //the error code (identifier)
};

struct LoggedUser{
	int userId; //id of user
	std::string username; //name of user
};

struct RelayServer{
	int port; //port of the relay
	int numberOfConnectedUsers; //amount of connected users
	std::string ipAddress; //ip address of the relay
};

struct nextNode{
	int port; //port of the next node
	std::string ipAdress; //ip adrress of the next node
	std::string encryptKey; //the public key of the next node
};

struct CommunicationData{
	int socketToEncrypt; //socket that is connected to the destination's side
	int socketToDecrypt; //socket that is connected to the client's side
	RSA_Communication* RsaHelper; //encryption and decryption helper
};

enum codes{
	loginCode = 0,
	signupCode = 1,

	accessApprovedCode = 1,
	wrongPasswordCode = 2,
	wrongUsernameCode = 3,
	signupSucceed = 4,
	usernameExists = 5,

	connectToNextNode = 40,

	weekPassword = 90,
	userAlreadyLoggedInCode = 91,

	databaseServerError = 100

};

enum databaseServerCodes{
	databaseServerLoginCode = 1,
	databaseServerSingupCode = 2
};

