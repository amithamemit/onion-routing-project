#include "JsonPacketSerialization.hpp"

unsigned char* JsonPacketSerialization::createLoginRequest(LoginRequest loginRequest, RSA_Communication* rsa){
    json j;
    j["username"] = loginRequest.username;
    j["password"] = loginRequest.password;
    int code = codes::loginCode;

    return rsa->encrypt(getJsonBufferRequest(j, code));
}

unsigned char* JsonPacketSerialization::createSignupRequest(SignupRequest signupRequest, RSA_Communication* rsa){
    json j;
    j["username"] = signupRequest.username;
    j["password"] = signupRequest.password;
    int code = codes::signupCode;


    return rsa->encrypt(getJsonBufferRequest(j, code));
}

unsigned char* JsonPacketSerialization::createPathRequest(PathRequest pathRequest, RSA_Communication* rsa)
{
	json j;
	j["lastNodeCountry"] = pathRequest.lastNodeCountry;
	j["relaysAmount"] = pathRequest.relaysAmount;
	int code = codes::createPath;

	return rsa->encrypt(getJsonBufferRequest(j, code));

}

unsigned char* JsonPacketSerialization::createNextNodeResult(nextNode nextRelay, RSA_Communication* rsa){
	json j;
	j["ip_adress"] = nextRelay.ipAdress;
    j["port"] = nextRelay.port;
    j["encrypt_key"] = nextRelay.encryptKey;
	int code = connectToNextNode;

	return rsa->encrypt(getJsonBufferRequest(j, code));
}

unsigned char* JsonPacketSerialization::createAvaliableLastRelaysRequest(RSA_Communication* rsa)
{
	json j;
	int code = codes::getLastRelays;
	return rsa->encrypt(getJsonBufferRequest(j, code));
}
unsigned char* JsonPacketSerialization::getJsonBufferRequest(json j, int code)
{
	unsigned char* buffer;
	//getting json string
	std::string jsonString = j.dump();

	// create a buffer to the response
	buffer = new unsigned char[1 + BYTES_LEN + jsonString.length()];
    // add the code to the buffer
    buffer[0] = code;
	unsigned char* byteLen = Helper::changeIntTo4ByteCharArray(jsonString.length());
	//add the bytes to the buffer
	for (int i = 1; i < BYTES_LEN + 1; i++) {
		buffer[i] = byteLen[i-1];
	}
	delete byteLen; //clean memory

	//add the json string to the buffer
	for (int i = 0; i < jsonString.length(); i++) {
		buffer[i + BYTES_LEN + 1] = (unsigned char)jsonString[i];
	}
	return buffer;
}