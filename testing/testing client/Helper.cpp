#include "Helper.hpp"

unsigned char* Helper::changeIntTo4ByteCharArray(int num){
	unsigned char* bytes = new unsigned char[4];
	//change the number to 4 byte format
	bytes[0] = (num >> 24) & 0xff;
	bytes[1] = (num >> 16) & 0xff;
	bytes[2] = (num >> 8) & 0xff;
	bytes[3] = num & 0xff;

	return bytes;
}

int Helper::convert4ByteToInt(unsigned char* bytes){
	return ((unsigned char)bytes[1] << 24) | ((unsigned char)bytes[2] << 16) | ((unsigned char)bytes[3] << 8) | (unsigned char)bytes[4]; //getting json string length
}



std::string Helper::getCurrentTime()
{
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	std::stringstream ss;
	ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
	return "[" + ss.str() + "] ";
}