#pragma once
#include <iostream>
#include "IRequestHandler.hpp"
#include "DataStructures.hpp"
#include "Helper.hpp"
#include "json.hpp"

using nlohmann::json;

class JsonPacketDeserialization
{
public:
	static LoginInfo getLoginInfo(unsigned char*);
	static SignupInfo getSignupInfo(unsigned char*);
	static LoginResult getDatabaseLoginResult(unsigned char*);
	static SignupResult getDatabaseSignupResult(unsigned char*);
	static nextNode getNextNode(unsigned char*);

private:
	static json getJsonFromBuffer(unsigned char* buffer);
	static int getMessageCode(unsigned char* buffer);
	static int getMessageLength(unsigned char* buffer);
	static json getJsonFromBuffer(unsigned char* buffer);
	static json getJsonFromDatabaseServerBuffer(unsigned char* buffer);
	
};