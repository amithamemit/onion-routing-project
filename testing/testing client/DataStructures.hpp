#pragma once
//#include "IRequestHandler.hpp"
//#include "RSA_Communication.hpp"

class IRequestHandler;

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
	int port;
	int numberOfConnectedUsers;
	std::string ipAddress;
};

struct nextNode{
	int port;
	std::string ipAdress;
	std::string encryptKey;
};

struct CommunicationData{
	int socketToEncrypt;
	int socketToDecrypt;
	//RSA_Communication* RsaHelper;
};

enum codes{
	loginCode = 0,
	signupCode = 1,

	accessApprovedCode = 1,
	wrongPasswordCode = 2,
	wrongUsernameCode = 3,
	signupSucceed = 4,
	usernameExists = 5,

	weekPassword = 90,
	userAlreadyLoggedInCode = 91,

	databaseServerError = 100

};

enum databaseServerCodes{
	databaseServerLoginCode = 1,
	databaseServerSingupCode = 2
};

