#pragma once
#include <iostream>
#include <vector>


struct LoginRequest{
    std::string username;
    std::string password;
};

struct SignupRequest{
    std::string username;
    std::string password;
};

struct signupAndLoginResponse
{
    int responseID;
};

struct nextNode{
	int port;
	std::string ipAdress;
	std::string encryptKey;
};

struct relayInfo
{
	std::string ipAdress;
	int port;
};

struct relaysPath //response for path request
{
    std::vector<nextNode> _path;
    std::string serverResponse;
};

struct PathRequest
{
    int relaysAmount;
    std::string lastNodeCountry;
};

struct avaliableLastNodes
{
	std::vector<std::string> _countries; //only relay location will be avaliable to client, no need to send ip and port 
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

	connectToNextNode = 40,

	weekPassword = 90,
	userAlreadyLoggedInCode = 91,

	databaseServerError = 100

};
