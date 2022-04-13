#pragma once
#include <iostream>
#include <string>
#include "DataStructures.hpp"
#include "Helper.hpp"
#include "json.hpp"
#include "RSA_Communication.hpp"

using nlohmann::json;


#define BYTES_LEN 4


class JsonPacketSerialization{
	public:
		static unsigned char* createLoginResult(LoginResult loginResult, RSA_Communication*);
		static unsigned char* createSignupResult(SignupResult signupResult, RSA_Communication*);
		
		static unsigned char* createErrorRequestResult(ErrorResult errorResult, RSA_Communication*);

		static unsigned char* createPublicKeyResponse(std::string publicKey);

		static unsigned char* createAvaliableLastRelaysResult(avaliableLastNodes lastRelays, RSA_Communication* rsa);
		static unsigned char* createRouteResult(relaysPath path, RSA_Communication* rsa);
	private:
		static unsigned char* getJsonBufferResponse(json j);

};