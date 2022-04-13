#pragma once
#include <iostream>
#include "RSA_Communication.hpp"
#include "DataStructures.hpp"
#include "Helper.hpp"
#include "json.hpp"

using nlohmann::json;

class JsonPacketDeserialization
{
public:
	static signupAndLoginResponse getLoginAndSignupResponse(unsigned char*, RSA_Communication* );
	static avaliableLastNodes avaliableLastRelaysResponse(unsigned char*, RSA_Communication*);
	static relaysPath getRouteResponse(unsigned char*, RSA_Communication*);

private:
    static int getMessageCode(unsigned char* buffer);
	static int getMessageLength(unsigned char* buffer);
	static json getJsonFromBuffer(unsigned char* buffer);
	static json getJsonFromDatabaseServerBuffer(unsigned char* buffer);
};