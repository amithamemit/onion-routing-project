#pragma once
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <chrono>  // chrono::system_clock
#include <ctime>   // localtime
#include <sstream> // stringstream
#include <iomanip> // put_time
#include <string>  // string
#include "DataStructures.hpp"
#include "JsonPacketSerialization.hpp"

class Helper{
    public:
        static unsigned char* changeIntTo4ByteCharArray(int);
        static int convert4ByteToInt(unsigned char* bytes);
        static RequestResult createErrorResult(std::string errorMessage, int errorCode);
        static std::string getCurrentTime();
};