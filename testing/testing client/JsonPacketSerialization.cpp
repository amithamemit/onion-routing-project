#include "JsonPacketSerialization.hpp"

/*
MESSAGE FORMAT: CODE(1 byte) MESSAGE LENGTH(4 bytes) JSON MESSAGE(n bytes)
*/

unsigned char* JsonPacketSerialization::createLoginResult(LoginResult loginResult){
	//create json message
	json j;
	j["code"] = loginResult.code;

	return getJsonBufferResponse(j);
}

unsigned char* JsonPacketSerialization::createNextNodeResult(nextNode nextRelay){
	json j;
	j["ip_adress"] = nextRelay.ipAdress;
    j["port"] = nextRelay.port;
    j["encrypt_key"] = nextRelay.encryptKey;

	return getJsonBufferResponse(j);
}

unsigned char* JsonPacketSerialization::createSignupResult(SignupResult signupResult){
	json j;
	j["code"] = signupResult.code;

	return getJsonBufferResponse(j);
}

unsigned char* JsonPacketSerialization::createErrorRequestResult(ErrorResult errorResult){
	json j;
	j["message"] = errorResult.errorMessage;
	j["code"] = errorResult.code;

	return getJsonBufferResponse(j);
}

unsigned char* JsonPacketSerialization::getJsonBufferResponse(json j)
{
	unsigned char* buffer;
	//getting json string
	std::string jsonString = j.dump();
	//create a buffer to the response
	buffer = new unsigned char[BYTES_LEN + jsonString.length()];

	// getting the json string length in 4 byte format and add it to the buffer
	unsigned char* byteLen = Helper::changeIntTo4ByteCharArray(jsonString.length());
	//add the bytes to the buffer
	for (int i = 0; i < BYTES_LEN; i++) {
		buffer[i] = byteLen[i];
	}
	delete byteLen; //clean memory

	//add the json string to the buffer
	for (int i = 0; i < jsonString.length(); i++) {
		buffer[i + BYTES_LEN] = (unsigned char)jsonString[i];
	}
	return buffer;
}