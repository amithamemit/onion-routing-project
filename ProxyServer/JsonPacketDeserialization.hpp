#pragma once
#include <iostream>
#include "IRequestHandler.hpp"
#include "DataStructures.hpp"
#include "Helper.hpp"
#include "json.hpp"
#include "RSA_Communication.hpp"

using nlohmann::json;

class JsonPacketDeserialization
{
public:
	static LoginInfo getLoginInfo(unsigned char*, RSA_Communication*);
	static SignupInfo getSignupInfo(unsigned char*, RSA_Communication*);
	static LoginResult getDatabaseLoginResult(unsigned char*, RSA_Communication*);
	static SignupResult getDatabaseSignupResult(unsigned char*, RSA_Communication*);
	static std::string getPublicKey(unsigned char*);
	static SocketData getSocketData(unsigned char*);
	static int getConnectUsersCount(unsigned char*);
	static PathRequest getPathRequest(unsigned char*);

private:
	static int getMessageCode(unsigned char* buffer);
	static int getMessageLength(unsigned char* buffer);
	static json getJsonFromBuffer(unsigned char* buffer);
	static json getJsonFromDatabaseServerBuffer(unsigned char* buffer);
	
};