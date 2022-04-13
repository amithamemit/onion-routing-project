#include "JsonPacketSerialization.hpp"

/*
MESSAGE FORMAT: CODE(1 byte) MESSAGE LENGTH(4 bytes) JSON MESSAGE(n bytes)
*/

/*
* this function creates a json reprersentation in text that contains the login response message
* input: login response
* output: json in text
*/
unsigned char* JsonPacketSerialization::createLoginResult(LoginResult loginResult){
	//create json message
	json j;
	j["code"] = loginResult.code;

	return getJsonBufferResponse(j);
}

/*
* this function creates a json reprersentation in text that contains the signup response message
* input: signup response
* output: json in text
*/
unsigned char* JsonPacketSerialization::createSignupResult(SignupResult signupResult){
	json j;
	j["code"] = signupResult.code;

	return getJsonBufferResponse(j);
}

/*
* this function creates a json reprersentation in text that contains the error result message
* input: error result
* output: json in text
*/
unsigned char* JsonPacketSerialization::createErrorRequestResult(ErrorResult errorResult){
	json j;
	j["message"] = errorResult.errorMessage;
	j["code"] = errorResult.code;

	return getJsonBufferResponse(j);
}

/*
* this function creates a json reprersentation in text that contains the amount of connected users result message
* input: amount of connected users result
* output: json in text
*/
unsigned char* JsonPacketSerialization::createUpdateUsersResult(int amountOfConnectedUsers){
	json j;
	j["connectedUsers"] = amountOfConnectedUsers;

	return getJsonBufferResponse(j);
}

/*
* this function creates a text representation of a json
* input: json
* output: text representation
*/
unsigned char* JsonPacketSerialization::getJsonBufferResponse(json j)
{
	unsigned char* buffer;
	//getting json string
	std::string jsonString = j.dump();
	//create a buffer to the response
	buffer = new unsigned char[BYTES_LEN + jsonString.length() + 1];

	// getting the json string length in 4 byte format and add it to the buffer
	unsigned char* byteLen = Helper::changeIntTo4ByteCharArray(jsonString.length());
	//add the bytes to the buffer
	for (int i = 1; i < BYTES_LEN + 1; i++) {
		buffer[i] = byteLen[i-1];
	}
	delete byteLen; //clean memory

	//add the json string to the buffer
	for (int i = 1; i < jsonString.length() + 1; i++) {
		buffer[i + BYTES_LEN] = (unsigned char)jsonString[i-1];
	}
	return buffer;
}