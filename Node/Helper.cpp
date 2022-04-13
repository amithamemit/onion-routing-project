#include "Helper.hpp"

/*
* this function converts a number to a hex representation
* input: number
* output: hex representation stored in an array
*/
unsigned char* Helper::changeIntTo4ByteCharArray(int num){
	unsigned char* bytes = new unsigned char[4]; //aarray to represent byte representation of number
	//change the number to 4 byte format
	bytes[0] = (num >> 24) & 0xff;
	bytes[1] = (num >> 16) & 0xff;
	bytes[2] = (num >> 8) & 0xff;
	bytes[3] = num & 0xff;

	return bytes;
}

/*
* this function converts the hex representation of a number to decimal representation
* input: hexadecimal number 
* output: number in decimal 
*/
int Helper::convert4ByteToInt(unsigned char* bytes){
	return ((unsigned char)bytes[1] << 24) | ((unsigned char)bytes[2] << 16) | ((unsigned char)bytes[3] << 8) | (unsigned char)bytes[4]; //getting json string length
}

/*
* this function returns an error response
* input: the error message and the error code
* output: the request result that represents the error
*/
RequestResult Helper::createErrorResult(std::string errorMessage, int errorCode)
{
	RequestResult requestResult{}; //the result to return (new handler and message)
	ErrorResult errorResult; //error result to Serialize to response
	errorResult.errorMessage = errorMessage;
	errorResult.code = errorCode;
	requestResult.buffer = JsonPacketSerialization::createErrorRequestResult(errorResult);
	return requestResult;
}

/*
* this function returns the current time in the template "%Y-%m-%d %X"  (year-month-day and %X  Parses the locale's date representation.)
* input: none
* output: current time in text
*/
std::string Helper::getCurrentTime()
{
	auto now = std::chrono::system_clock::now(); //current time
	auto in_time_t = std::chrono::system_clock::to_time_t(now); //current time converted to a type that can be converted to the template "%Y-%m-%d %X"

	std::stringstream ss; //string stream that will contain the time sorted in the template "%Y-%m-%d %X"
	ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X"); //converting the time to the desired template
	return "[" + ss.str() + "] ";
}