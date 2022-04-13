#pragma once
#include <iostream>
#include <string>
#include "DataStructures.hpp"
#include "Helper.hpp"
#include "json.hpp"


using nlohmann::json;


#define BYTES_LEN 4


class JsonPacketSerialization{
	public:
		static unsigned char* createLoginResult(LoginResult loginResult);
		static unsigned char* createSignupResult(SignupResult signupResult);
		
		static unsigned char* createErrorRequestResult(ErrorResult errorResult);

		static unsigned char* createUpdateUsersResult(int connectedUsers);

	private:
		static unsigned char* getJsonBufferResponse(json j);

};