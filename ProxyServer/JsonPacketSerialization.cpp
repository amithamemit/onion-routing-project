#include "JsonPacketSerialization.hpp"

/*
MESSAGE FORMAT: CODE(1 byte) MESSAGE LENGTH(4 bytes) JSON MESSAGE(n bytes)
*/

unsigned char* JsonPacketSerialization::createLoginResult(LoginResult loginResult, RSA_Communication* rsa){
	//create json message
	json j;
	j["code"] = loginResult.code;

	// return encrypted buffer
	return rsa->encrypt(getJsonBufferResponse(j));
}

unsigned char* JsonPacketSerialization::createSignupResult(SignupResult signupResult, RSA_Communication* rsa){
	json j;
	j["code"] = signupResult.code;

	return rsa->encrypt(getJsonBufferResponse(j));
}

unsigned char* JsonPacketSerialization::createErrorRequestResult(ErrorResult errorResult, RSA_Communication* rsa){
	json j;
	j["message"] = errorResult.errorMessage;
	j["code"] = errorResult.code;

	return rsa->encrypt(getJsonBufferResponse(j));
}

unsigned char* JsonPacketSerialization::createPublicKeyResponse(std::string publicKey){
	json j;
	j["publicKey"] = publicKey;

	return getJsonBufferResponse(j);
}

unsigned char* JsonPacketSerialization::createAvaliableLastRelaysResult(avaliableLastNodes lastRelays, RSA_Communication* rsa)
{
	json j;
	j["countries"] = lastRelays._countries;

	return getJsonBufferResponse(j); //might need to add rsa encrypt later.
}

unsigned char* JsonPacketSerialization::createRouteResult(relaysPath path, RSA_Communication* rsa)
{
	json j;
	j["path"] = {};
	j["server_response"] = path.serverResponse;

	for (auto vectorit = path._path.begin(); vectorit != path._path.end(); ++vectorit)
	{
		json nodeJson;
		nodeJson["ipAdress"] = (*vectorit).ipAdress;
		nodeJson["port"] = (*vectorit).port;
		nodeJson["encryptKey"] = (*vectorit).encryptKey;
		j["path"].push_back(nodeJson);
	}
	
	return getJsonBufferResponse(j); //might need to add rsa encrypt later.
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